/****************************************************************************
** $Id: rs_actionselectinvert.cpp 1062 2004-01-16 21:51:20Z andrew $
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

#include "rs_actionselectinvert.h"
#include "rs_selection.h"

RS_ActionSelectInvert::RS_ActionSelectInvert(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Invert Selection",
                    container, graphicView) {
}


QAction* RS_ActionSelectInvert::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Invert Selection"), tr("&Invert Selection"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Inverts the current selection"));
    return action;
}


void RS_ActionSelectInvert::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
    finish();
}

void RS_ActionSelectInvert::trigger() {
    RS_Selection s(*container, graphicView);
    s.invertSelection();

    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}

// EOF
