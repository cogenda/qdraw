/****************************************************************************
** $Id: rs_actioneditcopy.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actioneditcopy.h"
#include "rs_modification.h"

/**
 * Constructor.
 *
 * @param undo true for undo and false for redo.
 */
RS_ActionEditCopy::RS_ActionEditCopy(bool copy,
                                     RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionInterface("Edit Copy",
                    container, graphicView) {

    this->copy = copy;
}



RS_ActionEditCopy::~RS_ActionEditCopy() {}



QAction* RS_ActionEditCopy::createGUIAction(RS2::ActionType type, QObject* parent) {
    QAction* action;

    if (type==RS2::ActionEditCopy) {
        //QPixmap icon = QPixmap(editcopy_xpm);
        action = new QAction(tr("Copy"), QPixmap::fromMimeSource("editcopy2.png"), tr("&Copy"),
                             CTRL+Key_C, parent);
        action->setStatusTip(tr("Copies entities"
                                " to the clipboard"));
    } else {
        //icon = QPixmap(editcut_xpm);
        action = new QAction(tr("Cut"), QPixmap::fromMimeSource("editcut2.png"), tr("Cu&t"),
                             CTRL+Key_X, parent);
        action->setStatusTip(tr("Cuts entities "
                                " to the clipboard"));
    }
    return action;
}



void RS_ActionEditCopy::init(int status) {
    RS_ActionInterface::init(status);
    //trigger();
}



void RS_ActionEditCopy::trigger() {

    deleteSnapper();

    RS_Modification m(*container, graphicView);
    m.copy(referencePoint, !copy);

    //graphicView->redraw();
    finish();
    graphicView->killSelectActions();
    //init(getStatus()-1);
    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}


void RS_ActionEditCopy::mouseMoveEvent(RS_MouseEvent* e) {
    snapPoint(e);
}



void RS_ActionEditCopy::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionEditCopy::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    referencePoint = e->getCoordinate();
    trigger();
}



void RS_ActionEditCopy::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetReferencePoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionEditCopy::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionEditCopy::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}


// EOF
