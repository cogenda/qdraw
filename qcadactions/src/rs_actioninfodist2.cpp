/****************************************************************************
** $Id: rs_actioninfodist2.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actioninfodist2.h"

#include "rs_snapper.h"



RS_ActionInfoDist2::RS_ActionInfoDist2(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Info Dist2",
                           container, graphicView) {}


QAction* RS_ActionInfoDist2::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Distance Entity to Point"),
                                  tr("&Distance Entity to Point"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Measures the distance between an "
                            "entity and a point"));
    return action;
}

void RS_ActionInfoDist2::init(int status) {
    RS_ActionInterface::init(status);
}



void RS_ActionInfoDist2::trigger() {

    RS_DEBUG->print("RS_ActionInfoDist2::trigger()");
    deleteSnapper();

    if (point.valid && entity!=NULL) {
        double dist = entity->getDistanceToPoint(point);
        QString str;
        str.sprintf("%.6f", dist);
        RS_DIALOGFACTORY->commandMessage(tr("Distance: %1").arg(str));
    }
}



void RS_ActionInfoDist2::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionInfoDist2::mouseMoveEvent begin");

    switch (getStatus()) {
    case SetEntity:
        //entity = catchEntity(e);
        break;

    case SetPoint:
        if (entity!=NULL) {
            point = snapPoint(e);
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionInfoDist2::mouseMoveEvent end");
}



void RS_ActionInfoDist2::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {

        switch (getStatus()) {
        case SetEntity:
            entity = catchEntity(e);
            if (entity!=NULL) {
                setStatus(SetPoint);
            }
            break;

        case SetPoint: {
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
        init(getStatus()-1);
    }
}


void RS_ActionInfoDist2::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    if (getStatus()==SetPoint && entity!=NULL) {
        point = e->getCoordinate();
        graphicView->moveRelativeZero(point);
        trigger();
        setStatus(SetEntity);
    }
}



void RS_ActionInfoDist2::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetEntity:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify entity"),
            tr("Cancel"));
        break;
    case SetPoint:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify point"),
            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionInfoDist2::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionInfoDist2::updateToolBar() {
    switch (getStatus()) {
    case SetPoint:
    case SetEntity:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarInfo);
        break;
    }
}


// EOF
