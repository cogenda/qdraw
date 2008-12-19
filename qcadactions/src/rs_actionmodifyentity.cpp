/****************************************************************************
** $Id: rs_actionmodifyentity.cpp 1149 2004-09-19 11:06:35Z andrew $
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

#include "rs_actionmodifyentity.h"

#include "rs_snapper.h"



RS_ActionModifyEntity::RS_ActionModifyEntity(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Modify Entity", container, graphicView) {

    en = NULL;
}


QAction* RS_ActionModifyEntity::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Properties"), tr("&Properties"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Modify Entity Properties"));
    return action;
}

void RS_ActionModifyEntity::trigger() {
    if (en!=NULL) {
        RS_Entity* clone = en->clone();
        if (RS_DIALOGFACTORY->requestModifyEntityDialog(clone)) {
            container->addEntity(clone);

            graphicView->deleteEntity(en);
			en->setSelected(false);

			clone->setSelected(false);
            graphicView->drawEntity(clone);

            if (document!=NULL) {
                document->startUndoCycle();

                document->addUndoable(clone);
                en->setUndoState(true);
                document->addUndoable(en);

                document->endUndoCycle();
            }
            RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
        } else {
            delete clone;
        }

    } else {
        RS_DEBUG->print("RS_ActionModifyEntity::trigger: Entity is NULL\n");
    }
}



void RS_ActionModifyEntity::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    } else {
        en = catchEntity(e);
        trigger();
    }
}



void RS_ActionModifyEntity::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::SelectCursor);
}

// EOF
