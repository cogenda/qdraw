/****************************************************************************
** $Id: rs_actioneditundo.cpp 1149 2004-09-19 11:06:35Z andrew $
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

#include "rs_actioneditundo.h"
#include "rs_snapper.h"

/**
 * Constructor.
 *
 * @param undo true for undo and false for redo.
 */
RS_ActionEditUndo::RS_ActionEditUndo(bool undo,
                                     RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionInterface("Edit Undo",
                    container, graphicView) {

    this->undo = undo;
}



RS_ActionEditUndo::~RS_ActionEditUndo() {}


QAction* RS_ActionEditUndo::createGUIAction(RS2::ActionType type, QObject* parent) {
    QAction* action;
    if (type==RS2::ActionEditUndo) {
        //icon = QPixmap(editundo_xpm);
        action = new QAction(tr("Undo"),
                             QPixmap::fromMimeSource("undo2.png"), tr("&Undo"),
                             CTRL+Key_Z, parent);
        action->setStatusTip(tr("Undoes last action"));
    } else {
        //icon = QPixmap(editredo_xpm);
        action = new QAction(tr("Redo"),
                             QPixmap::fromMimeSource("redo2.png"), tr("&Redo"),
                             CTRL+SHIFT+Key_Z, parent);
        action->setStatusTip(tr("Redoes last action"));
    }

    return action;
}

void RS_ActionEditUndo::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}



void RS_ActionEditUndo::trigger() {
    if (undo) {
        document->undo();
    } else {
        document->redo();
    }

    document->updateInserts();

    graphicView->redraw();

    finish();
    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}


// EOF
