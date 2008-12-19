/****************************************************************************
** $Id: rs_actiondimdiametric.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actiondimdiametric.h"

#include "rs_creation.h"
#include "rs_snapper.h"
#include "rs_dialogfactory.h"



RS_ActionDimDiametric::RS_ActionDimDiametric(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionDimension("Draw Diametric Dimensions",
                    container, graphicView) {
    reset();
}


QAction* RS_ActionDimDiametric::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Diametric"), tr("&Diametric"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Diametric Dimension"));

    return action;
}

void RS_ActionDimDiametric::reset() {
    RS_ActionDimension::reset();

    edata = RS_DimDiametricData(RS_Vector(false),
                                0.0);
    entity = NULL;
    pos = RS_Vector(false);
    RS_DIALOGFACTORY->requestOptions(this, true, true);
}



void RS_ActionDimDiametric::trigger() {
    RS_PreviewActionInterface::trigger();

    preparePreview();
    if (entity!=NULL) {
        RS_DimDiametric* newEntity = NULL;

        newEntity = new RS_DimDiametric(container,
                                        data,
                                        edata);

        newEntity->setLayerToActive();
        newEntity->setPenToActive();
        newEntity->update();
        container->addEntity(newEntity);

        // upd. undo list:
        if (document!=NULL) {
            document->startUndoCycle();
            document->addUndoable(newEntity);
            document->endUndoCycle();
        }
        deleteSnapper();
        RS_Vector rz = graphicView->getRelativeZero();
        graphicView->moveRelativeZero(RS_Vector(0.0,0.0));
        graphicView->drawEntity(newEntity);
        graphicView->moveRelativeZero(rz);

    } else {
        RS_DEBUG->print("RS_ActionDimDiametric::trigger:"
                        " Entity is NULL\n");
    }
}



void RS_ActionDimDiametric::preparePreview() {
    if (entity!=NULL) {
        double radius=0.0;
        RS_Vector center = RS_Vector(false);
        if (entity->rtti()==RS2::EntityArc) {
            radius = ((RS_Arc*)entity)->getRadius();
            center = ((RS_Arc*)entity)->getCenter();
        } else if (entity->rtti()==RS2::EntityCircle) {
            radius = ((RS_Circle*)entity)->getRadius();
            center = ((RS_Circle*)entity)->getCenter();
        }
        double angle = center.angleTo(pos);

        data.definitionPoint.setPolar(radius, angle+M_PI);
        data.definitionPoint += center;

        edata.definitionPoint.setPolar(radius, angle);
        edata.definitionPoint += center;
    }
}



void RS_ActionDimDiametric::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDimDiametric::mouseMoveEvent begin");

    RS_Vector mouse(graphicView->toGraphX(e->x()),
                    graphicView->toGraphY(e->y()));

    switch (getStatus()) {
    case SetEntity:
        entity = catchEntity(e, RS2::ResolveAll);
        break;

    case SetPos:
        if (entity!=NULL) {
            pos = snapPoint(e);

            preparePreview();
            RS_DimDiametric* d = new RS_DimDiametric(preview, data, edata);
            d->update();

            deletePreview();
            clearPreview();
            preview->addEntity(d);
            drawPreview();
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionDimDiametric::mouseMoveEvent end");
}



void RS_ActionDimDiametric::mouseReleaseEvent(RS_MouseEvent* e) {

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case SetEntity: {
                RS_Entity* en = catchEntity(e, RS2::ResolveAll);
                if (en!=NULL) {
                    if (en->rtti()==RS2::EntityArc ||
                            en->rtti()==RS2::EntityCircle) {

                        entity = en;
                        RS_Vector center;
                        if (entity->rtti()==RS2::EntityArc) {
                            center =
                                ((RS_Arc*)entity)->getCenter();
                        } else if (entity->rtti()==RS2::EntityCircle) {
                            center =
                                ((RS_Circle*)entity)->getCenter();
                        }
                        graphicView->moveRelativeZero(center);
                        setStatus(SetPos);
                    } else {
                        RS_DIALOGFACTORY->commandMessage(tr("Not a circle "
                                                            "or arc entity"));
                    }
                }
            }
            break;

        case SetPos: {
                RS_CoordinateEvent ce(snapPoint(e));
                coordinateEvent(&ce);
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        clearPreview();
        init(getStatus()-1);
    }

}



void RS_ActionDimDiametric::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    switch (getStatus()) {
    case SetPos:
        pos = e->getCoordinate();
        trigger();
        reset();
        setStatus(SetEntity);
        break;

    default:
        break;
    }
}



void RS_ActionDimDiametric::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    // setting new text label:
    if (getStatus()==SetText) {
        setText(c);
        RS_DIALOGFACTORY->requestOptions(this, true, true);
        graphicView->enableCoordinateInput();
        setStatus(lastStatus);
        return;
    }

    // command: text
    if (checkCommand("text", c)) {
        lastStatus = (Status)getStatus();
        graphicView->disableCoordinateInput();
        setStatus(SetText);
    }

    // setting angle
    if (getStatus()==SetPos) {
        bool ok;
        double a = RS_Math::eval(c, &ok);
        if (ok==true) {
            pos.setPolar(1.0, RS_Math::deg2rad(a));
            pos += data.definitionPoint;
            trigger();
            reset();
            setStatus(SetEntity);
        } else {
            RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
        }
        return;
    }
}



RS_StringList RS_ActionDimDiametric::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetEntity:
    case SetPos:
        cmd += command("text");
        break;

    default:
        break;
    }

    return cmd;
}


void RS_ActionDimDiametric::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetEntity:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select arc or circle entity"),
                                            tr("Cancel"));
        break;
    case SetPos:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify dimension line location"), tr("Cancel"));
        break;
    case SetText:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter dimension text:"), "");
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDimDiametric::showOptions() {
    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}



void RS_ActionDimDiametric::hideOptions() {
    RS_ActionInterface::hideOptions();

    //RS_DIALOGFACTORY->requestDimDiametricOptions(edata, false);
    RS_DIALOGFACTORY->requestOptions(this, false);
}



// EOF
