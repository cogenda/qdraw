/****************************************************************************
** $Id: rs_actionmodifyrotate.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionmodifyrotate.h"

#include "rs_snapper.h"



RS_ActionModifyRotate::RS_ActionModifyRotate(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Rotate Entities",
                           container, graphicView) {}


QAction* RS_ActionModifyRotate::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Rotate"), tr("&Rotate"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Rotate Entities"));
    return action;
}

void RS_ActionModifyRotate::init(int status) {
    RS_ActionInterface::init(status);
}



void RS_ActionModifyRotate::trigger() {

    RS_DEBUG->print("RS_ActionModifyRotate::trigger()");

    RS_Modification m(*container, graphicView);
    m.rotate(data);

    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionModifyRotate::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyRotate::mouseMoveEvent begin");

    if (getStatus()==SetReferencePoint) {
        RS_Vector mouse = snapPoint(e);
        switch (getStatus()) {
        case SetReferencePoint:
            referencePoint = mouse;
            break;

        default:
            break;
        }
    }

    RS_DEBUG->print("RS_ActionModifyRotate::mouseMoveEvent end");
}



void RS_ActionModifyRotate::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionModifyRotate::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector pos = e->getCoordinate();

    switch (getStatus()) {
    case SetReferencePoint:
        referencePoint = pos;
        setStatus(ShowDialog);
        if (RS_DIALOGFACTORY->requestRotateDialog(data)) {
            data.center = referencePoint;
            trigger();
            finish();
        }
        break;

    default:
        break;
    }
}



void RS_ActionModifyRotate::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetReferencePoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyRotate::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyRotate::updateToolBar() {
    switch (getStatus()) {
    case SetReferencePoint:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
        break;
    }
}


// EOF
