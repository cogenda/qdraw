/****************************************************************************
** $Id: rs_actiondrawlinerectangle.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actiondrawlinerectangle.h"
#include "rs_snapper.h"



RS_ActionDrawLineRectangle::RS_ActionDrawLineRectangle(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw rectangles",
                           container, graphicView) {
    reset();
}



RS_ActionDrawLineRectangle::~RS_ActionDrawLineRectangle() {}

QAction* RS_ActionDrawLineRectangle::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        QAction* action = new QAction(tr("Rectangle"), tr("&Rectangle"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Draw rectangles"));
		return action;
}


void RS_ActionDrawLineRectangle::reset() {
    for (int i=0; i<4; ++i) {
        data[i] = RS_LineData(RS_Vector(false),
                              RS_Vector(false));
    }
}



void RS_ActionDrawLineRectangle::init(int status) {
    RS_PreviewActionInterface::init(status);

    reset();
}



void RS_ActionDrawLineRectangle::trigger() {
    RS_PreviewActionInterface::trigger();

    RS_Line* line[4];
    preparePreview();

    // create and add rectangle:
    for (int i=0; i<4; ++i) {
        line[i] = new RS_Line(container,
                              data[i]);
        line[i]->setLayerToActive();
        line[i]->setPenToActive();
        container->addEntity(line[i]);
    }

    // upd. undo list:
    if (document!=NULL) {
        document->startUndoCycle();
        for (int i=0; i<4; ++i) {
            document->addUndoable(line[i]);
        }
        document->endUndoCycle();
    }

    // upd. view
    deleteSnapper();
    graphicView->moveRelativeZero(RS_Vector(0.0,0.0));
    for (int i=0; i<4; ++i) {
        graphicView->drawEntity(line[i]);
        RS_DEBUG->print("RS_ActionDrawLineRectangle::trigger():"
                        " line added: %d",
                        line[i]->getId());
    }
    graphicView->moveRelativeZero(corner2);
}



void RS_ActionDrawLineRectangle::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawLineRectangle::mouseMoveEvent begin");

    RS_Vector mouse = snapPoint(e);
    if (getStatus()==SetCorner2 && corner1.valid) {
        corner2 = mouse;
        deletePreview();
        clearPreview();

        preparePreview();

        for (int i=0; i<4; ++i) {
            preview->addEntity(new RS_Line(preview, data[i]));
        }
        drawPreview();
    }

    RS_DEBUG->print("RS_ActionDrawLineRectangle::mouseMoveEvent end");
}


void RS_ActionDrawLineRectangle::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionDrawLineRectangle::preparePreview() {
    data[0] = RS_LineData(corner1, RS_Vector(corner2.x, corner1.y));
    data[1] = RS_LineData(RS_Vector(corner2.x, corner1.y), corner2);
    data[2] = RS_LineData(corner2, RS_Vector(corner1.x, corner2.y));
    data[3] = RS_LineData(RS_Vector(corner1.x, corner2.y), corner1);
}


void RS_ActionDrawLineRectangle::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetCorner1:
        corner1 = mouse;
        graphicView->moveRelativeZero(mouse);
        setStatus(SetCorner2);
        break;

    case SetCorner2:
        corner2 = mouse;
        trigger();
        setStatus(SetCorner1);
        break;

    default:
        break;
    }
}



void RS_ActionDrawLineRectangle::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }
}



RS_StringList RS_ActionDrawLineRectangle::getAvailableCommands() {
    RS_StringList cmd;
    return cmd;
}


void RS_ActionDrawLineRectangle::updateMouseButtonHints() {
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
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}


void RS_ActionDrawLineRectangle::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}


void RS_ActionDrawLineRectangle::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
        }
    }
}

// EOF
