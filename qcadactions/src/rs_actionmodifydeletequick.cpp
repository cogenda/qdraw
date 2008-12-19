/****************************************************************************
** $Id: rs_actionmodifydeletequick.cpp 1144 2004-08-18 17:07:54Z andrew $
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

#include "rs_actionmodifydeletequick.h"

#include "rs_actionselectsingle.h"
#include "rs_snapper.h"
#include "rs_point.h"



RS_ActionModifyDeleteQuick::RS_ActionModifyDeleteQuick(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionInterface("Quick Delete Entities",
                    container, graphicView) {

    en = NULL;
}


QAction* RS_ActionModifyDeleteQuick::createGUIAction(RS2::ActionType /*type*/, QObject* parent) {
#ifdef __APPLE__
	QKeySequence s = Key_Backspace;
#else
	QKeySequence s = Key_Delete;
#endif

        QAction* action = new QAction(tr("Delete selected"), 
							tr("&Delete selected"),
                             s, parent);
        action->setStatusTip(tr("Delete selected entities"));
		return action;
}


/**
 * Deletes all entities that were selected.
 */
void RS_ActionModifyDeleteQuick::trigger() {

    RS_DEBUG->print("RS_ActionModifyDeleteQuick::trigger()");

    if (en!=NULL) {
        RS_DEBUG->print("Entity found");
        RS_EntityContainer* parent = en->getParent();
        if (parent!=NULL) {
            en->setSelected(false);
            graphicView->deleteEntity(en);
            en->changeUndoState();

            if (document) {
                document->startUndoCycle();
                document->addUndoable(en);
                document->endUndoCycle();
            }
        }

    	RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    } else {
        RS_DEBUG->print("RS_ActionModifyDeleteQuick::mousePressEvent:"
                        " Entity is NULL\n");
    }
}



void RS_ActionModifyDeleteQuick::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    } else {
        en = catchEntity(e);
        trigger();
    }
}



void RS_ActionModifyDeleteQuick::updateMouseButtonHints() {
    switch (getStatus()) {
    case 0:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Pick entity to delete"),
                                       tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyDeleteQuick::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::DelCursor);
}

// EOF
