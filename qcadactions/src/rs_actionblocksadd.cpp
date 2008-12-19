/****************************************************************************
** $Id: rs_actionblocksadd.cpp 1062 2004-01-16 21:51:20Z andrew $
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

#include "rs_actionblocksadd.h"

#include "rs_graphic.h"
#include "rs_dialogfactory.h"


RS_ActionBlocksAdd::RS_ActionBlocksAdd(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_ActionInterface("Add Block", container, graphicView) {}



QAction* RS_ActionBlocksAdd::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Add Block"), tr("&Add Block"),
                             QKeySequence(), NULL);
    action->setStatusTip(tr("Add Block"));
    return action;
}



void RS_ActionBlocksAdd::trigger() {
    RS_DEBUG->print("adding block");
    //RS_Block* block = new RS_Block(container, "", RS_Vector(0.0,0.0));
    if (graphic!=NULL) {
		RS_BlockList* blockList = graphic->getBlockList();
		if (blockList!=NULL) {
			RS_BlockData d = 
				RS_DIALOGFACTORY->requestNewBlockDialog(blockList);
			if (d.isValid()) {
        		graphic->addBlock(new RS_Block(container, d));
			}
		}
    }
    finish();
}



void RS_ActionBlocksAdd::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
