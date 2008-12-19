/****************************************************************************
** $Id: rs_actionmodifytrimamount.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionmodifytrimamount.h"

#include "rs_snapper.h"


RS_ActionModifyTrimAmount::RS_ActionModifyTrimAmount(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionInterface("Trim Entity by a given amount",
                    container, graphicView) {

    trimEntity = NULL;
    trimCoord = RS_Vector(false);
    distance = 0.0;
}

QAction* RS_ActionModifyTrimAmount::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        QAction* action = new QAction(tr("Lengthen"), tr("&Lengthen"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Lengthen by a given amount"));
		return action;
}


void RS_ActionModifyTrimAmount::init(int status) {
    RS_ActionInterface::init(status);

    snapMode = RS2::SnapFree;
    snapRes = RS2::RestrictNothing;
}



void RS_ActionModifyTrimAmount::trigger() {

    RS_DEBUG->print("RS_ActionModifyTrimAmount::trigger()");

    if (trimEntity!=NULL && trimEntity->isAtomic()) {

        RS_Modification m(*container, graphicView);
        m.trimAmount(trimCoord, (RS_AtomicEntity*)trimEntity, distance);

        trimEntity = NULL;
        setStatus(ChooseTrimEntity);

        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
}



void RS_ActionModifyTrimAmount::mouseReleaseEvent(RS_MouseEvent* e) {

    trimCoord = graphicView->toGraph(e->x(), e->y());
    trimEntity = catchEntity(e);

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case ChooseTrimEntity:
            if (trimEntity!=NULL && trimEntity->isAtomic()) {
                trigger();
            } else {
                if (trimEntity==NULL) {
                    RS_DIALOGFACTORY->commandMessage(
                        tr("No entity found. "));
                } else if (trimEntity->rtti()==RS2::EntityInsert) {
                    RS_DIALOGFACTORY->commandMessage(
                        tr("The chosen Entity is in a block. "
                           "Please edit the block."));
                } else {
                    RS_DIALOGFACTORY->commandMessage(
                        tr("The chosen Entity is not an atomic entity "
                           "or cannot be trimmed."));
                }
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionModifyTrimAmount::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case ChooseTrimEntity: {
            bool ok;
            double d = RS_Math::eval(c, &ok);
            if (ok==true) {
                distance = d;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(ChooseTrimEntity);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionModifyTrimAmount::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case ChooseTrimEntity:
        break;
    default:
        break;
    }

    return cmd;
}


void RS_ActionModifyTrimAmount::showOptions() {
    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}



void RS_ActionModifyTrimAmount::hideOptions() {
    RS_ActionInterface::hideOptions();

    RS_DIALOGFACTORY->requestOptions(this, false);
}


void RS_ActionModifyTrimAmount::updateMouseButtonHints() {
    switch (getStatus()) {
    case ChooseTrimEntity:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Select entity to trim or enter distance:"),
            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyTrimAmount::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyTrimAmount::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
}


// EOF
