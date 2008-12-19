/****************************************************************************
** $Id: rs_actiondimaligned.cpp 1134 2004-07-13 23:26:13Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "rs_actiondimaligned.h"

#include "rs_snapper.h"
#include "rs_constructionline.h"
#include "rs_dialogfactory.h"



RS_ActionDimAligned::RS_ActionDimAligned(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionDimension("Draw aligned dimensions",
                    container, graphicView) {

    reset();
}



RS_ActionDimAligned::~RS_ActionDimAligned() {}

QAction* RS_ActionDimAligned::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        QAction* action = new QAction(tr("Aligned"), tr("&Aligned"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Aligned Dimension"));

		return action;
}


void RS_ActionDimAligned::reset() {
    RS_ActionDimension::reset();

    edata = RS_DimAlignedData(RS_Vector(false),
                              RS_Vector(false));
    lastStatus = SetExtPoint1;
    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, true, true);
    }
}



void RS_ActionDimAligned::trigger() {
    RS_ActionDimension::trigger();

    preparePreview();
    graphicView->moveRelativeZero(data.definitionPoint);

	//data.text = getText();
    RS_DimAligned* dim =
        new RS_DimAligned(container, data, edata);
    dim->setLayerToActive();
    dim->setPenToActive();
    dim->update();
    container->addEntity(dim);

    // upd. undo list:
    if (document!=NULL) {
        document->startUndoCycle();
        document->addUndoable(dim);
        document->endUndoCycle();
    }

    deleteSnapper();
    RS_Vector rz = graphicView->getRelativeZero();
    graphicView->moveRelativeZero(RS_Vector(0.0,0.0));
    graphicView->drawEntity(dim);
    graphicView->moveRelativeZero(rz);
    drawSnapper();

    RS_DEBUG->print("RS_ActionDimAligned::trigger():"
                    " dim added: %d", dim->getId());
}



void RS_ActionDimAligned::preparePreview() {
    RS_Vector dirV;
    dirV.setPolar(100.0,
                  edata.extensionPoint1.angleTo(
                      edata.extensionPoint2)
                  +M_PI/2.0);
    RS_ConstructionLine cl(NULL,
                           RS_ConstructionLineData(
                               edata.extensionPoint2,
                               edata.extensionPoint2+dirV));

    data.definitionPoint =
        cl.getNearestPointOnEntity(data.definitionPoint);
}



void RS_ActionDimAligned::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDimAligned::mouseMoveEvent begin");

    RS_Vector mouse = snapPoint(e);

    switch (getStatus()) {
    case SetExtPoint1:
        break;

    case SetExtPoint2:
        if (edata.extensionPoint1.valid) {
            deletePreview();
            clearPreview();
            preview->addEntity(
                new RS_Line(preview,
                            RS_LineData(edata.extensionPoint1, mouse))
            );
            drawPreview();
        }
        break;

    case SetDefPoint:
        if (edata.extensionPoint1.valid && edata.extensionPoint2.valid) {
            deletePreview();
            clearPreview();
            data.definitionPoint = mouse;

            preparePreview();

			//data.text = getText();
            RS_DimAligned* dim = new RS_DimAligned(preview, data, edata);
            dim->update();
            preview->addEntity(dim);
            drawPreview();
        }
        break;

	default:
		break;
    }

    RS_DEBUG->print("RS_ActionDimAligned::mouseMoveEvent end");
}



void RS_ActionDimAligned::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionDimAligned::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector pos = e->getCoordinate();

    switch (getStatus()) {
    case SetExtPoint1:
        edata.extensionPoint1 = pos;
        graphicView->moveRelativeZero(pos);
        setStatus(SetExtPoint2);
        break;

    case SetExtPoint2:
        edata.extensionPoint2 = pos;
        graphicView->moveRelativeZero(pos);
        setStatus(SetDefPoint);
        break;

    case SetDefPoint:
        data.definitionPoint = pos;
        trigger();
        reset();
        setStatus(SetExtPoint1);
        break;

    default:
        break;
    }
}



void RS_ActionDimAligned::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    switch (getStatus()) {
    case SetText: {
            setText(c);
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->requestOptions(this, true, true);
            }
            setStatus(lastStatus);
            graphicView->enableCoordinateInput();
        }
        break;

    default:
        if (checkCommand("text", c)) {
            lastStatus = (Status)getStatus();
            graphicView->disableCoordinateInput();
            setStatus(SetText);
        }
        break;
    }
}



RS_StringList RS_ActionDimAligned::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetExtPoint1:
    case SetExtPoint2:
    case SetDefPoint:
        cmd += command("text");
        break;

    default:
        break;
    }

    return cmd;
}



void RS_ActionDimAligned::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetExtPoint1:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify first extension line origin"),
                tr("Cancel"));
            break;
        case SetExtPoint2:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify second extension line origin"),
                tr("Back"));
            break;
        case SetDefPoint:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify dimension line location"),
                tr("Back"));
            break;
        case SetText:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Enter dimension text:"), "");
            break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionDimAligned::hideOptions() {
    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, false);
    }

    RS_ActionDimension::hideOptions();
}



void RS_ActionDimAligned::showOptions() {
    RS_ActionDimension::showOptions();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, true);
    }
}


// EOF
