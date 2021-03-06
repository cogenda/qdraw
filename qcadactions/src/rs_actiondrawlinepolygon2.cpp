/****************************************************************************
** $Id: rs_actiondrawlinepolygon2.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actiondrawlinepolygon2.h"

#include "rs_creation.h"
#include "rs_snapper.h"



RS_ActionDrawLinePolygon2::RS_ActionDrawLinePolygon2(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw Polygons", container, graphicView) {

    corner1 = RS_Vector(false);
    corner2 = RS_Vector(false);

    number = 3;
}

QAction* RS_ActionDrawLinePolygon2::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Polygon"), tr("Polygo&n (Cor,Cor)"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Draw polygon with two corners"));
    return action;
}


void RS_ActionDrawLinePolygon2::trigger() {
    RS_PreviewActionInterface::trigger();

    deleteSnapper();
    deletePreview();
    clearPreview();

    RS_Creation creation(container, graphicView);
    bool ok = creation.createPolygon2(corner1, corner2, number);

    if (!ok) {
        RS_DEBUG->print("RS_ActionDrawLinePolygon2::trigger:"
                        " No polygon added\n");
    }
}



void RS_ActionDrawLinePolygon2::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawLinePolygon2::mouseMoveEvent begin");

    RS_Vector mouse = snapPoint(e);

    switch (getStatus()) {
    case SetCorner1:
        break;

    case SetCorner2:
        if (corner1.valid) {
            corner2 = mouse;
            deletePreview();
            clearPreview();

            RS_Creation creation(preview, NULL, false);
            creation.createPolygon2(corner1, corner2, number);

            drawPreview();
        }
        break;

    default:
        break;
    }
}



void RS_ActionDrawLinePolygon2::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        clearPreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionDrawLinePolygon2::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetCorner1:
        corner1 = mouse;
        setStatus(SetCorner2);
        graphicView->moveRelativeZero(mouse);
        break;

    case SetCorner2:
        corner2 = mouse;
        trigger();
        break;

    default:
        break;
    }
}



void RS_ActionDrawLinePolygon2::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetCorner1:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify first corner"),
                                                tr("Cancel"));
            break;

        case SetCorner2:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify second corner"),
                                                tr("Back"));
            break;

        case SetNumber:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Number:"), tr("Back"));
            break;

        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionDrawLinePolygon2::showOptions() {
    RS_ActionInterface::showOptions();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, true);
    }
}



void RS_ActionDrawLinePolygon2::hideOptions() {
    RS_ActionInterface::hideOptions();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, false);
    }
}



void RS_ActionDrawLinePolygon2::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    switch (getStatus()) {
    case SetCorner1:
    case SetCorner2:
        if (checkCommand("number", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetNumber);
        }
        break;

    case SetNumber: {
            bool ok;
            int n = c.toInt(&ok);
            if (ok==true) {
                if (n>0 && n<10000) {
                    number = n;
                } else {
                    if (RS_DIALOGFACTORY!=NULL) {
                        RS_DIALOGFACTORY->commandMessage(tr("Not a valid number. "
                                                            "Try 1..9999"));
                    }
                }
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression."));
                }
            }
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->requestOptions(this, true, true);
            }
            setStatus(lastStatus);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawLinePolygon2::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetCorner1:
    case SetCorner2:
        cmd += command("number");
        break;
    default:
        break;
    }

    return cmd;
}



void RS_ActionDrawLinePolygon2::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawLinePolygon2::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
        }
    }
}


// EOF
