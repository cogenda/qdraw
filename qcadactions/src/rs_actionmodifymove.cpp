/****************************************************************************
** $Id: rs_actionmodifymove.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionmodifymove.h"

#include "rs_snapper.h"



RS_ActionModifyMove::RS_ActionModifyMove(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Move Entities",
                           container, graphicView) {}


QAction* RS_ActionModifyMove::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Move / Copy"), tr("&Move / Copy"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Move or copy entities one or multiple times"));
    return action;
}

void RS_ActionModifyMove::init(int status) {
    RS_ActionInterface::init(status);
}



void RS_ActionModifyMove::trigger() {

    RS_DEBUG->print("RS_ActionModifyMove::trigger()");

    RS_Modification m(*container, graphicView);
    m.move(data);

    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionModifyMove::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyMove::mouseMoveEvent begin");

    if (getStatus()==SetReferencePoint ||
            getStatus()==SetTargetPoint) {

        RS_Vector mouse = snapPoint(e);
        switch (getStatus()) {
        case SetReferencePoint:
            referencePoint = mouse;
            break;

        case SetTargetPoint:
            if (referencePoint.valid) {
                targetPoint = mouse;

                deletePreview();
                clearPreview();
                preview->addSelectionFrom(*container);
                preview->move(targetPoint-referencePoint);
                drawPreview();
            }
            break;

        default:
            break;
        }
    }

    RS_DEBUG->print("RS_ActionModifyMove::mouseMoveEvent end");
}



void RS_ActionModifyMove::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}

void RS_ActionModifyMove::coordinateEvent(RS_CoordinateEvent* e) {

    if (e==NULL) {
        return;
    }

    RS_Vector pos = e->getCoordinate();

    switch (getStatus()) {
    case SetReferencePoint:
        referencePoint = pos;
        graphicView->moveRelativeZero(referencePoint);
        setStatus(SetTargetPoint);
        break;

    case SetTargetPoint:
        targetPoint = pos;
        graphicView->moveRelativeZero(targetPoint);
        setStatus(ShowDialog);
        if (RS_DIALOGFACTORY->requestMoveDialog(data)) {
            data.offset = targetPoint - referencePoint;
            trigger();
            finish();
        }
        break;

    default:
        break;
    }
}


void RS_ActionModifyMove::updateMouseButtonHints() {
    switch (getStatus()) {
        /*case Select:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Pick entities to move"),
                                           tr("Cancel"));
            break;*/
    case SetReferencePoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Cancel"));
        break;
    case SetTargetPoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify target point"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyMove::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyMove::updateToolBar() {
    switch (getStatus()) {
        /*case Select:
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSelect);
            break;*/
    case SetReferencePoint:
    case SetTargetPoint:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
        break;
    }
}


// EOF
