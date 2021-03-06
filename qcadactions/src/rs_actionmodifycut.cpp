/****************************************************************************
** $Id: rs_actionmodifycut.cpp 2420 2005-07-01 16:47:35Z andrew $
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

#include "rs_actionmodifycut.h"

#include "rs_snapper.h"


RS_ActionModifyCut::RS_ActionModifyCut(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_ActionInterface("Cut Entity",
                    container, graphicView) {

    cutEntity = NULL;
    cutCoord = RS_Vector(false);
}

QAction* RS_ActionModifyCut::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Divide"), tr("&Divide"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Cut Entities"));
    return action;
}


void RS_ActionModifyCut::init(int status) {
    RS_ActionInterface::init(status);
}



void RS_ActionModifyCut::trigger() {

    RS_DEBUG->print("RS_ActionModifyCut::trigger()");

    if (cutEntity!=NULL && cutEntity->isAtomic() && cutCoord.valid &&
            cutEntity->isPointOnEntity(cutCoord)) {

        cutEntity->setHighlighted(false);
        graphicView->drawEntity(cutEntity);

        RS_Modification m(*container, graphicView);
        m.cut(cutCoord, (RS_AtomicEntity*)cutEntity);

        cutEntity = NULL;
        cutCoord = RS_Vector(false);
        setStatus(ChooseCutEntity);

        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
}



void RS_ActionModifyCut::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyCut::mouseMoveEvent begin");

    switch (getStatus()) {
    case ChooseCutEntity:
        break;

    case SetCutCoord:
        snapPoint(e);
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionModifyTrim::mouseMoveEvent end");
}


void RS_ActionModifyCut::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case ChooseCutEntity:
            cutEntity = catchEntity(e);
            if (cutEntity==NULL) {
                RS_DIALOGFACTORY->commandMessage(tr("No Entity found."));
            } else if (cutEntity->rtti()!=RS2::EntityLine &&
                       cutEntity->rtti()!=RS2::EntityArc &&
                       cutEntity->rtti()!=RS2::EntityCircle &&
                       cutEntity->rtti()!=RS2::EntityEllipse) {

                RS_DIALOGFACTORY->commandMessage(
                    tr("Entity must be a line, arc, circle or ellipse."));
            } else {
                cutEntity->setHighlighted(true);
                graphicView->drawEntity(cutEntity);
                setStatus(SetCutCoord);
            }
            break;

        case SetCutCoord:
            cutCoord = snapPoint(e);
            if (cutEntity==NULL) {
                RS_DIALOGFACTORY->commandMessage(tr("No Entity found."));
            } else if (!cutCoord.valid) {
                RS_DIALOGFACTORY->commandMessage(tr("Cutting point is invalid."));
            } else if (!cutEntity->isPointOnEntity(cutCoord)) {
                RS_DIALOGFACTORY->commandMessage(
                    tr("Cutting point is not on entity."));
            } else {
                deleteSnapper();
                trigger();
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deleteSnapper();
        if (cutEntity!=NULL) {
            cutEntity->setHighlighted(false);
            graphicView->drawEntity(cutEntity);
        }
        init(getStatus()-1);
    }
}



void RS_ActionModifyCut::updateMouseButtonHints() {
    switch (getStatus()) {
    case ChooseCutEntity:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify entity to cut"),
                                            tr("Cancel"));
        break;
    case SetCutCoord:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify cutting point"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyCut::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyCut::updateToolBar() {

    switch (getStatus()) {
    case SetCutCoord:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    case ChooseCutEntity:
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
        break;
    }
}


// EOF
