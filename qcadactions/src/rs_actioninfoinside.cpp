/****************************************************************************
** $Id: rs_actioninfoinside.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actioninfoinside.h"
#include "rs_information.h"
#include "rs_snapper.h"



RS_ActionInfoInside::RS_ActionInfoInside(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Info Inside",
                    container, graphicView) {

    contour = new RS_EntityContainer(NULL, false);

    for (RS_Entity* e=container.firstEntity(); e!=NULL;
            e=container.nextEntity()) {
        if (e->isSelected()) {
            contour->addEntity(e);
        }
    }
}


RS_ActionInfoInside::~RS_ActionInfoInside() {
    delete contour;
}


QAction* RS_ActionInfoInside::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Point inside contour"),
                                  tr("&Point inside contour"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Checks if a given point is inside the "
                            "selected contour"));

    return action;
}

void RS_ActionInfoInside::trigger() {
    deleteSnapper();
    bool onContour = false;
    if (RS_Information::isPointInsideContour(pt, contour, &onContour)) {
        RS_DIALOGFACTORY->commandMessage(tr("Point is inside selected contour."));
    } else {
        RS_DIALOGFACTORY->commandMessage(tr("Point is outside selected contour."));
    }
    finish();
}



void RS_ActionInfoInside::mouseMoveEvent(RS_MouseEvent* e) {
    //RS_Vector mouse = snapPoint(e);
    //bool onContour = false;
    /*if (RS_Information::isPointInsideContour(mouse, contour, &onContour)) {
    } else {
    }*/
}



void RS_ActionInfoInside::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deleteSnapper();
        init(getStatus()-1);
    } else {
        pt = snapPoint(e);
        trigger();
    }
}



void RS_ActionInfoInside::updateMouseButtonHints() {
    switch (getStatus()) {
    case 0:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify point"),
                                            tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionInfoInside::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionInfoInside::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarInfo);
    }
}

// EOF
