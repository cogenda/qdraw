/****************************************************************************
** $Id: rs_actionsetrelativezero.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actionsetrelativezero.h"

#include "rs_snapper.h"
#include "rs_point.h"



RS_ActionSetRelativeZero::RS_ActionSetRelativeZero(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Set the relative Zero",
                           container, graphicView) {}


QAction* RS_ActionSetRelativeZero::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Set Relative Zero"), tr("&Set Relative Zero"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Set position of the Relative Zero point"));
    return action;
}


void RS_ActionSetRelativeZero::trigger() {
    bool wasLocked = graphicView->isRelativeZeroLocked();
    if (pt.valid) {
        graphicView->lockRelativeZero(false);
        graphicView->moveRelativeZero(pt);
        graphicView->lockRelativeZero(wasLocked);
    }
    finish();
}



void RS_ActionSetRelativeZero::mouseMoveEvent(RS_MouseEvent* e) {
    snapPoint(e);
}



void RS_ActionSetRelativeZero::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deleteSnapper();
        init(getStatus()-1);
    } else {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    }
}



void RS_ActionSetRelativeZero::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    pt = e->getCoordinate();
    trigger();
}



void RS_ActionSetRelativeZero::updateMouseButtonHints() {
    switch (getStatus()) {
    case 0:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Set relative Zero"), tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionSetRelativeZero::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionSetRelativeZero::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    }
}

// EOF
