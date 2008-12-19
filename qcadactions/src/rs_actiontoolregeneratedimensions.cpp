/****************************************************************************
** $Id: rs_actiontoolregeneratedimensions.cpp 1074 2004-02-29 00:30:50Z andrew $
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

#include "rs_actiontoolregeneratedimensions.h"

#include "rs_information.h"
#include "rs_dimension.h"



RS_ActionToolRegenerateDimensions::RS_ActionToolRegenerateDimensions(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Tool Regen Dim",
                    container, graphicView) {}


QAction* RS_ActionToolRegenerateDimensions::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Regenerate Dimension Entities"),
                                  tr("&Regenerate Dimension Entities"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Regenerates all Dimension Entities"));

    return action;
}

void RS_ActionToolRegenerateDimensions::init(int status) {
    RS_ActionInterface::init(status);

    trigger();
}



void RS_ActionToolRegenerateDimensions::trigger() {

    RS_DEBUG->print("RS_ActionToolRegenerateDimensions::trigger()");

    int num = 0;
    for (RS_Entity* e = container->firstEntity(RS2::ResolveNone);
            e != NULL;
            e = container->nextEntity(RS2::ResolveNone)) {

        if (RS_Information::isDimension(e->rtti()) && e->isVisible()) {
			num++;
			if (((RS_Dimension*)e)->getLabel()==";;") {
				((RS_Dimension*)e)->setLabel("");
			}
            ((RS_Dimension*)e)->update(true);
        }
    }

    if (num>0) {
    	graphicView->redraw();
        RS_DIALOGFACTORY->commandMessage(
            tr("Regenerated %1 dimension entities").arg(num));
    } else {
        RS_DIALOGFACTORY->commandMessage(tr("No dimension entities found"));
    }

    finish();
}


// EOF
