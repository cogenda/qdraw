/****************************************************************************
** $Id: rs_actionblocksremove.cpp 2372 2005-04-29 11:44:54Z andrew $
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

#include "rs_actionblocksremove.h"

#include "rs_block.h"
#include "rs_graphic.h"
#include "rs_insert.h"
#include "rs_dialogfactory.h"



RS_ActionBlocksRemove::RS_ActionBlocksRemove(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Remove Block", container, graphicView) {}


QAction* RS_ActionBlocksRemove::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        QAction* action = new QAction(tr("Remove Block"), tr("&Remove Block"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Remove Block"));

		return action;
}

void RS_ActionBlocksRemove::trigger() {
    RS_DEBUG->print("RS_ActionBlocksRemove::trigger");

    if (graphic!=NULL) {
        RS_Block* block =
            RS_DIALOGFACTORY->requestBlockRemovalDialog(graphic->getBlockList());

        // list of containers that might refer to the block via inserts:
        RS_PtrList<RS_EntityContainer> containerList;
        containerList.append(graphic);
        RS_BlockList* blkLst = graphic->getBlockList();
        for (uint bi=0; bi<blkLst->count(); bi++) {
            containerList.append(blkLst->at(bi));
        }
        
        if (block!=NULL) {
            
            for (RS_EntityContainer* cont = containerList.first();
                cont!=NULL;
                cont=containerList.next()) {
        
                // remove all inserts from the graphic:
                bool done;
                do {
                    done = true;
                    for (RS_Entity* e=cont->firstEntity(RS2::ResolveNone);
                            e!=NULL;
                            e=cont->nextEntity(RS2::ResolveNone)) {

                        if (e->rtti()==RS2::EntityInsert) {
                            RS_Insert* ins = (RS_Insert*)e;
                            if (ins->getName()==block->getName()) {
                                cont->removeEntity(ins);
                                done = false;
                                break;
                            }
                        }
                    }
                } while (!done);
            }

			// close all windows that are editing this block:
        	if (RS_DIALOGFACTORY!=NULL) {
	            RS_DIALOGFACTORY->closeEditBlockWindow(block);
    	    }

            // Now remove the block from the block list:
            graphic->removeBlock(block);
            graphic->updateInserts();
			graphicView->redraw();
        }
    }

    finish();
    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionBlocksRemove::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
