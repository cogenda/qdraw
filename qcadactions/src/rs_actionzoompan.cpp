/****************************************************************************
** $Id: rs_actionzoompan.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actionzoompan.h"
#include "rs_snapper.h"
#include "rs_point.h"


RS_ActionZoomPan::RS_ActionZoomPan(RS_EntityContainer& container,
                                   RS_GraphicView& graphicView)
        :RS_ActionInterface("Zoom Pan", container, graphicView) {}


QAction* RS_ActionZoomPan::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Pan Zoom"), QPixmap::fromMimeSource("zoompan.png"),
                                  tr("&Pan Zoom"), QKeySequence(), NULL);
    action->setStatusTip(tr("Realtime Panning"));
    return action;
}


void RS_ActionZoomPan::init(int status) {
    RS_ActionInterface::init(status);
    snapMode = RS2::SnapFree;
    snapRes = RS2::RestrictNothing;
    //v1 = v2 = RS_Vector(false);
    x1 = y1 = x2 = y2 = -1;
    //graphicView->saveView();
}



void RS_ActionZoomPan::trigger() {
    /*if (v1.valid && v2.valid) {
        graphicView->zoomPan(v2-v1);
        v1 = v2;
}*/
    if (x1>=0) {
        graphicView->zoomPan(x2-x1, y2-y1);
        x1 = x2;
        y1 = y2;
    }
}



void RS_ActionZoomPan::mouseMoveEvent(RS_MouseEvent* e) {
    //v2 = snapPoint(e);
    x2 = e->x();
    y2 = e->y();
    //if (getStatus()==1 && graphicView->toGuiDX((v2-v1).magnitude())>10) {
    if (getStatus()==1 && (abs(x2-x1)>7 || abs(y2-y1)>7)) {
        trigger();
    }
}



void RS_ActionZoomPan::mousePressEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::MidButton ||
            RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        //v1 = snapPoint(e);
        x1 = e->x();
        y1 = e->y();
        setStatus(1);
    }
}



void RS_ActionZoomPan::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::MidButton) {
        init(-1);
    } else {
        setStatus(0);
    }

    //RS_DEBUG->print("RS_ActionZoomPan::mousePressEvent(): %f %f", v1.x, v1.y);
}



void RS_ActionZoomPan::updateMouseCursor() {
#ifndef __APPLE__
    graphicView->setMouseCursor(RS2::SizeAllCursor);
#endif
}


// EOF
