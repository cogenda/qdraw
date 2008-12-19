/****************************************************************************
** $Id: rs_actioninfodist.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actioninfodist.h"

#include "rs_snapper.h"



RS_ActionInfoDist::RS_ActionInfoDist(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Info Dist",
                           container, graphicView) {}


QAction* RS_ActionInfoDist::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        QAction* action = new QAction(tr("Distance Point to Point"),
                             tr("&Distance Point to Point"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Measures the distance between two points"));
		return action;
}


void RS_ActionInfoDist::init(int status) {
    RS_ActionInterface::init(status);

}



void RS_ActionInfoDist::trigger() {

    RS_DEBUG->print("RS_ActionInfoDist::trigger()");

    if (point1.valid && point2.valid) {
        double dist = point1.distanceTo(point2);
        QString str;
        str.sprintf("%.6f", dist);
        RS_DIALOGFACTORY->commandMessage(tr("Distance: %1").arg(str));
    }
}



void RS_ActionInfoDist::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionInfoDist::mouseMoveEvent begin");

    if (getStatus()==SetPoint1 ||
            getStatus()==SetPoint2) {

        RS_Vector mouse = snapPoint(e);
        switch (getStatus()) {
        case SetPoint1:
            break;

        case SetPoint2:
            if (point1.valid) {
                point2 = mouse;

                deletePreview();
                clearPreview();

                preview->addEntity(new RS_Line(preview,
                                               RS_LineData(point1,
                                                           point2)));

                drawPreview();
            }
            break;

        default:
            break;
        }
    }

    RS_DEBUG->print("RS_ActionInfoDist::mouseMoveEvent end");
}



void RS_ActionInfoDist::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionInfoDist::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetPoint1:
        point1 = mouse;
    	graphicView->moveRelativeZero(point1);
        setStatus(SetPoint2);
        break;

    case SetPoint2:
        if (point1.valid) {
            point2 = mouse;
            deletePreview();
            clearPreview();
    		graphicView->moveRelativeZero(point2);
            trigger();
            setStatus(SetPoint1);
        }
        break;

    default:
        break;
    }
}


void RS_ActionInfoDist::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetPoint1:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify first point of distance"),
            tr("Cancel"));
        break;
    case SetPoint2:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify second point of distance"),
            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionInfoDist::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionInfoDist::updateToolBar() {
    switch (getStatus()) {
    case SetPoint1:
    case SetPoint2:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarInfo);
        break;
    }
}


// EOF
