/****************************************************************************
** $Id: rs_actionselectcontour.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actionselectcontour.h"

#include "rs_selection.h"
#include "rs_snapper.h"



RS_ActionSelectContour::RS_ActionSelectContour(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Select Contours", container, graphicView) {

    en = NULL;
}

QAction* RS_ActionSelectContour::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("(De-)Select Contour"), tr("(De-)Select &Contour"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("(De-)Selects connected entities"));
    return action;
}


void RS_ActionSelectContour::trigger() {
    if (en!=NULL) {
        if (en->isAtomic()) {
            RS_Selection s(*container, graphicView);
            s.selectContour(en);

            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
            }
        } else {
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->commandMessage(
                    tr("Entity must be an Atomic Entity."));
            }
        }
    } else {
        RS_DEBUG->print("RS_ActionSelectContour::trigger: Entity is NULL\n");
    }
}



void RS_ActionSelectContour::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    } else {
        en = catchEntity(e);
        trigger();
    }
}



void RS_ActionSelectContour::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::SelectCursor);
}

// EOF
