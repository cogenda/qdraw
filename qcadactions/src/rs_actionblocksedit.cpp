/****************************************************************************
** $Id: rs_actionblocksedit.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionblocksedit.h"

#include "rs_graphic.h"
#include "rs_dialogfactory.h"



RS_ActionBlocksEdit::RS_ActionBlocksEdit(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Edit Block", container, graphicView) {}


QAction* RS_ActionBlocksEdit::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        QAction* action = new QAction(tr("Edit Block"), tr("&Edit Block"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Edit Block"));
		return action;
}


void RS_ActionBlocksEdit::trigger() {
    RS_DEBUG->print("edit block");
    if (graphic!=NULL) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->requestEditBlockWindow(graphic->getBlockList());
        }
    } else {
        RS_DEBUG->print(RS_Debug::D_WARNING,
        	"RS_ActionBlocksEdit::trigger(): graphic is NULL");
    }
    finish();
}



void RS_ActionBlocksEdit::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

