/****************************************************************************
** $Id: rs_actionmodifytrim.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionmodifytrim.h"

#include "rs_snapper.h"


/**
 * @param both Trim both entities.
 */
RS_ActionModifyTrim::RS_ActionModifyTrim(RS_EntityContainer& container,
        RS_GraphicView& graphicView, bool both)
        :RS_PreviewActionInterface("Trim Entity",
                           container, graphicView) {

    trimEntity = NULL;
    trimCoord = RS_Vector(false);
    limitEntity = NULL;
    limitCoord = RS_Vector(false);
    this->both = both;
}

QAction* RS_ActionModifyTrim::createGUIAction(RS2::ActionType type, QObject* /*parent*/) {
    QAction* action;
	
	switch (type) {
	default:
	case RS2::ActionModifyTrim:
		action = new QAction(tr("Trim"), tr("&Trim"),
	                                  QKeySequence(), NULL);
	    action->setStatusTip(tr("Trim Entities"));
		break;

	case RS2::ActionModifyTrim2:
        action = new QAction(tr("Trim Two"), tr("&Trim Two"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Trim two Entities"));
		break;
	}
	
    return action;
}


void RS_ActionModifyTrim::init(int status) {
    RS_ActionInterface::init(status);

    snapMode = RS2::SnapFree;
    snapRes = RS2::RestrictNothing;

}



void RS_ActionModifyTrim::trigger() {

    RS_DEBUG->print("RS_ActionModifyTrim::trigger()");

    if (trimEntity!=NULL && trimEntity->isAtomic() &&
            limitEntity!=NULL /* && limitEntity->isAtomic()*/) {

        RS_Modification m(*container, graphicView);
        m.trim(trimCoord, (RS_AtomicEntity*)trimEntity,
               limitCoord, /*(RS_AtomicEntity*)*/limitEntity,
               both);

        trimEntity = NULL;
        if (both) {
            limitEntity->setHighlighted(false);
            graphicView->drawEntity(limitEntity);
            setStatus(ChooseLimitEntity);
        } else {
            setStatus(ChooseTrimEntity);
        }

        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
}



void RS_ActionModifyTrim::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyTrim::mouseMoveEvent begin");

    RS_Vector mouse = graphicView->toGraph(e->x(), e->y());
    RS_Entity* se = catchEntity(e);

    switch (getStatus()) {
    case ChooseLimitEntity:
        limitCoord = mouse;
        limitEntity = se;
        break;

    case ChooseTrimEntity:
        trimCoord = mouse;
        trimEntity = se;
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionModifyTrim::mouseMoveEvent end");
}



void RS_ActionModifyTrim::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {

        RS_Vector mouse = graphicView->toGraph(e->x(), e->y());
        RS_Entity* se = catchEntity(e);

        switch (getStatus()) {
        case ChooseLimitEntity:
            limitCoord = mouse;
            limitEntity = se;
            if (limitEntity!=NULL /*&& limitEntity->isAtomic()*/) {
                limitEntity->setHighlighted(true);
                graphicView->drawEntity(limitEntity);
                setStatus(ChooseTrimEntity);
            }
            break;

        case ChooseTrimEntity:
            trimCoord = mouse;
            trimEntity = se;
            if (trimEntity!=NULL && trimEntity->isAtomic()) {
                trigger();
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        if (limitEntity!=NULL) {
            limitEntity->setHighlighted(false);
            graphicView->drawEntity(limitEntity);
        }
        init(getStatus()-1);
    }
}



void RS_ActionModifyTrim::updateMouseButtonHints() {
    switch (getStatus()) {
    case ChooseLimitEntity:
        if (both) {
            RS_DIALOGFACTORY->updateMouseWidget(tr("Select first trim entity"),
                                                tr("Cancel"));
        } else {
            RS_DIALOGFACTORY->updateMouseWidget(tr("Select limiting entity"),
                                                tr("Back"));
        }
        break;
    case ChooseTrimEntity:
        if (both) {
            RS_DIALOGFACTORY->updateMouseWidget(tr("Select second trim entity"),
                                                tr("Cancel"));
        } else {
            RS_DIALOGFACTORY->updateMouseWidget(tr("Select entity to trim"),
                                                tr("Back"));
        }
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyTrim::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyTrim::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
}


