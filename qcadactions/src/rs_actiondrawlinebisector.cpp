/****************************************************************************
** $Id: rs_actiondrawlinebisector.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actiondrawlinebisector.h"

#include "rs_creation.h"
#include "rs_snapper.h"



RS_ActionDrawLineBisector::RS_ActionDrawLineBisector(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw Bisectors", container, graphicView) {

    bisector = NULL;
    length = 10.0;
    line1 = NULL;
    line2 = NULL;
    number = 1;
    coord1 = RS_Vector(false);
    coord2 = RS_Vector(false);
    lastStatus = SetLine1;
}

QAction* RS_ActionDrawLineBisector::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Bisector"), tr("&Bisector"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Draw bisectors"));
    return action;
}


void RS_ActionDrawLineBisector::trigger() {
    RS_PreviewActionInterface::trigger();

    //if (bisector!=NULL) {
    RS_Creation creation(container, graphicView);
    creation.createBisector(coord1,
                            coord2,
                            length,
                            number,
                            line1,
                            line2);
    /*RS_Entity* newEntity = NULL;

    newEntity = new RS_Line(container,
                            bisector->getData());

    if (newEntity!=NULL) {
        newEntity->setLayerToActive();
        newEntity->setPenToActive();
        container->addEntity(newEntity);

        // upd. undo list:
        if (document!=NULL) {
            document->startUndoCycle();
            document->addUndoable(newEntity);
            document->endUndoCycle();
        }
        graphicView->drawEntity(newEntity);
        setStatus(SetLine1);
}
    //reset();
    delete bisector;
    bisector = NULL;
    */
    /*} else {
        RS_DEBUG->print("RS_ActionDrawLineBisector::trigger:"
                        " Entity is NULL\n");
}*/
}



void RS_ActionDrawLineBisector::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawLineBisector::mouseMoveEvent begin");

    RS_Vector mouse = RS_Vector(graphicView->toGraphX(e->x()),
                                graphicView->toGraphY(e->y()));

    switch (getStatus()) {
    case SetLine1:
        break;

    case SetLine2: {
            coord2 = mouse;
            RS_Entity* en = catchEntity(e, RS2::ResolveAll);
            if (en!=NULL && en->rtti()==RS2::EntityLine) {
                line2 = (RS_Line*)en;

                deletePreview();
                clearPreview();

                RS_Creation creation(preview, NULL, false);
                creation.createBisector(coord1,
                                        coord2,
                                        length,
                                        number,
                                        line1,
                                        line2);
                drawPreview();
            }
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionDrawLineBisector::mouseMoveEvent end");
}



void RS_ActionDrawLineBisector::mouseReleaseEvent(RS_MouseEvent* e) {

    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        clearPreview();
        init(getStatus()-1);
    } else {

        RS_Vector mouse = RS_Vector(graphicView->toGraphX(e->x()),
                                    graphicView->toGraphY(e->y()));

        switch (getStatus()) {
        case SetLine1: {
                coord1 = mouse;
                RS_Entity* en = catchEntity(e, RS2::ResolveAll);
                if (en!=NULL && en->rtti()==RS2::EntityLine) {
                    line1 = (RS_Line*)en;
                }
            }
            setStatus(SetLine2);
            break;

        case SetLine2:
            coord2 = mouse;
            trigger();
            setStatus(SetLine1);
            break;
        }
    }

}


void RS_ActionDrawLineBisector::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetLine1:
    case SetLine2:
        lastStatus = (Status)getStatus();
        if (checkCommand("length", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            setStatus(SetLength);
        } else if (checkCommand("number", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            setStatus(SetNumber);
        }
        break;

    case SetLength: {
            bool ok;
            double l = RS_Math::eval(c, &ok);
            if (ok==true) {
                length = l;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

    case SetNumber: {
            bool ok;
            int n = (int)RS_Math::eval(c, &ok);
            if (ok==true) {
                number = n;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;


    default:
        break;
    }
}



RS_StringList RS_ActionDrawLineBisector::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetLine1:
    case SetLine2:
        cmd += command("length");
        cmd += command("number");
        break;
    default:
        break;
    }

    return cmd;
}


void RS_ActionDrawLineBisector::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetLine1:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select first line"),
                                            tr("Cancel"));
        break;
    case SetLine2:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select second line"),
                                            tr("Back"));
        break;
    case SetLength:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter bisector length:"),
                                            tr("Back"));
        break;
    case SetNumber:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter number of bisectors:"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDrawLineBisector::showOptions() {
    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}



void RS_ActionDrawLineBisector::hideOptions() {
    RS_ActionInterface::hideOptions();

    RS_DIALOGFACTORY->requestOptions(this, false);
}



void RS_ActionDrawLineBisector::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawLineBisector::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
}



// EOF
