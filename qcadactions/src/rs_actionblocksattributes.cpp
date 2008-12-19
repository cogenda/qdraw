/****************************************************************************
** $Id: rs_actionblocksattributes.cpp 1062 2004-01-16 21:51:20Z andrew $
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

#include "rs_actionblocksattributes.h"

#include "rs_graphic.h"
#include "rs_dialogfactory.h"



RS_ActionBlocksAttributes::RS_ActionBlocksAttributes(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionInterface("Edit Block Attributes", container, graphicView) {}




QAction* RS_ActionBlocksAttributes::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {

    QAction* action = new QAction(tr("Rename Block"),
                                  tr("&Rename Block"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Rename Block and all Inserts"));
    return action;
}



void RS_ActionBlocksAttributes::trigger() {
    RS_DEBUG->print("editing block attributes");

    if (graphic!=NULL && RS_DIALOGFACTORY!=NULL) {
        RS_Block* block = graphic->getActiveBlock();
        RS_BlockList* blockList = graphic->getBlockList();
        if (blockList!=NULL && block!=NULL) {
            RS_String oldName = block->getName();

            RS_BlockData d;
            d = RS_DIALOGFACTORY->requestBlockAttributesDialog(
                    blockList);

            if (d.isValid()) {

                RS_String newName = d.name;
                blockList->rename(block, newName);

                // update the name of all inserts:
                graphic->renameInserts(oldName, newName);

                graphic->addBlockNotification();
            }
        }

    }
    finish();
}



void RS_ActionBlocksAttributes::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
