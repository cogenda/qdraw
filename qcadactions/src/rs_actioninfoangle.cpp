/****************************************************************************
** $Id: rs_actioninfoangle.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actioninfoangle.h"

#include "rs_information.h"
#include "rs_snapper.h"



RS_ActionInfoAngle::RS_ActionInfoAngle(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Info Angle",
                           container, graphicView) {}


QAction* RS_ActionInfoAngle::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Angle between two lines"),
                                  tr("&Angle between two lines"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Measures the angle between two lines"));
    return action;
}


void RS_ActionInfoAngle::init(int status) {
    RS_ActionInterface::init(status);

}



void RS_ActionInfoAngle::trigger() {

    RS_DEBUG->print("RS_ActionInfoAngle::trigger()");
    deleteSnapper();

    if (entity1!=NULL && entity2!=NULL) {
        RS_VectorSolutions sol =
            RS_Information::getIntersection(entity1, entity2, false);

        if (sol.hasValid()) {
            intersection = sol.get(0);

            if (intersection.valid && point1.valid && point2.valid) {
                double angle1 = intersection.angleTo(point1);
                double angle2 = intersection.angleTo(point2);
                double angle = fabs(angle2-angle1);

                QString str;
                str.sprintf("%.6f", RS_Math::rad2deg(angle));
                RS_DIALOGFACTORY->commandMessage(tr("Angle: %1%2")
                                                 .arg(str).arg(QChar(0xB0)));
            }
        } else {
            RS_DIALOGFACTORY->commandMessage(tr("Lines are parallel"));
        }
    }
}



void RS_ActionInfoAngle::mouseMoveEvent(RS_MouseEvent* /*e*/) {
    RS_DEBUG->print("RS_ActionInfoAngle::mouseMoveEvent begin");

    switch (getStatus()) {
    case SetEntity1:
        break;

    case SetEntity2:
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionInfoAngle::mouseMoveEvent end");
}



void RS_ActionInfoAngle::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {

        RS_Vector mouse(graphicView->toGraphX(e->x()),
                        graphicView->toGraphY(e->y()));

        switch (getStatus()) {
        case SetEntity1:
            entity1 = catchEntity(e);
            if (entity1!=NULL && entity1->rtti()==RS2::EntityLine) {
                point1 = entity1->getNearestPointOnEntity(mouse);
                setStatus(SetEntity2);
            }
            break;

        case SetEntity2:
            entity2 = catchEntity(e);
            if (entity2!=NULL && entity2->rtti()==RS2::EntityLine) {
                point2 = entity2->getNearestPointOnEntity(mouse);
                trigger();
                setStatus(SetEntity1);
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionInfoAngle::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetEntity1:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify first line"),
            tr("Cancel"));
        break;
    case SetEntity2:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify second line"),
            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionInfoAngle::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionInfoAngle::updateToolBar() {
    switch (getStatus()) {
    case SetEntity1:
    case SetEntity2:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarInfo);
        break;
    }
}


// EOF
