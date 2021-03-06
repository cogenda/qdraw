/****************************************************************************
** $Id: rs_actiondimlinear.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actiondimlinear.h"
#include "rs_snapper.h"
#include "rs_constructionline.h"
#include "rs_dialogfactory.h"

/**
 * Constructor.
 *
 * @param angle Initial angle in rad.
 * @param fixedAngle true: The user can't change the angle.
 *                   false: The user can change the angle in a option widget.
 */
RS_ActionDimLinear::RS_ActionDimLinear(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView,
                                       double angle,
                                       bool fixedAngle)
        :RS_ActionDimension("Draw linear dimensions",
                    container, graphicView) {

    edata.angle = angle;
    this->fixedAngle = fixedAngle;

    lastStatus = SetExtPoint1;

    reset();
}



RS_ActionDimLinear::~RS_ActionDimLinear() {}


QAction* RS_ActionDimLinear::createGUIAction(RS2::ActionType type, QObject* /*parent*/) {
    QAction* action;

    switch (type) {
    default:
    case RS2::ActionDimLinear:
        action = new QAction(tr("Linear"), tr("&Linear"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Linear Dimension"));
        break;

    case RS2::ActionDimLinearHor:
        action = new QAction(tr("Horizontal"), tr("&Horizontal"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Horizontal Dimension"));
        break;

    case RS2::ActionDimLinearVer:
        action = new QAction(tr("Vertical"), tr("&Vertical"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Vertical Dimension"));
        break;
    }

    return action;
}


void RS_ActionDimLinear::reset() {
    RS_ActionDimension::reset();

    edata = RS_DimLinearData(RS_Vector(false),
                             RS_Vector(false),
                             (fixedAngle ? edata.angle : 0.0), 0.0);

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, true, true);
    }
}



void RS_ActionDimLinear::trigger() {
    RS_ActionDimension::trigger();

    preparePreview();
    RS_DimLinear* dim = new RS_DimLinear(container, data, edata);
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

    RS_DEBUG->print("RS_ActionDimLinear::trigger():"
                    " dim added: %d", dim->getId());
}


void RS_ActionDimLinear::preparePreview() {
    RS_Vector dirV;
    dirV.setPolar(100.0, edata.angle+M_PI/2.0);

    RS_ConstructionLine cl(
        NULL,
        RS_ConstructionLineData(
            edata.extensionPoint2,
            edata.extensionPoint2+dirV));

    data.definitionPoint =
        cl.getNearestPointOnEntity(data.definitionPoint);

}



void RS_ActionDimLinear::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDimLinear::mouseMoveEvent begin");

    RS_Vector mouse = snapPoint(e);

    switch (getStatus()) {
    case SetExtPoint1:
        break;

    case SetExtPoint2:
        if (edata.extensionPoint1.valid) {
            deletePreview();
            clearPreview();
            preview->addEntity(new RS_Line(preview,
                                           RS_LineData(edata.extensionPoint1,
                                                       mouse)));
            drawPreview();
        }
        break;

    case SetDefPoint:
        if (edata.extensionPoint1.valid && edata.extensionPoint2.valid) {
            deletePreview();
            clearPreview();
            data.definitionPoint = mouse;

            preparePreview();

            RS_DimLinear* dim = new RS_DimLinear(preview, data, edata);
            dim->update();
            preview->addEntity(dim);
            drawPreview();
        }
        break;
    }

    RS_DEBUG->print("RS_ActionDimLinear::mouseMoveEvent end");
}



void RS_ActionDimLinear::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionDimLinear::coordinateEvent(RS_CoordinateEvent* e) {
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



void RS_ActionDimLinear::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    switch (getStatus()) {
    case SetText:
        setText(c);
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->requestOptions(this, true, true);
        }
        graphicView->enableCoordinateInput();
        setStatus(lastStatus);
        break;

    case SetAngle: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                setAngle(RS_Math::deg2rad(a));
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                }
            }
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->requestOptions(this, true, true);
            }
            setStatus(lastStatus);
        }
        break;

    default:
        lastStatus = (Status)getStatus();
        deleteSnapper();
        deletePreview();
        clearPreview();
        if (checkCommand("text", c)) {
            graphicView->disableCoordinateInput();
            setStatus(SetText);
            return;
        } else if (!fixedAngle && (checkCommand("angle", c))) {
            setStatus(SetAngle);
        }
        break;
    }
}



RS_StringList RS_ActionDimLinear::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetExtPoint1:
    case SetExtPoint2:
    case SetDefPoint:
        cmd += command("text");
        if (!fixedAngle) {
            cmd += command("angle");
        }
        break;

    default:
        break;
    }

    return cmd;
}


void RS_ActionDimLinear::updateMouseButtonHints() {
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
        case SetAngle:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Enter dimension line angle:"), "");
            break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionDimLinear::showOptions() {
    RS_ActionInterface::showOptions();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, true, true);
    }
}



void RS_ActionDimLinear::hideOptions() {
    RS_ActionInterface::hideOptions();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, false);
    }
}



// EOF
