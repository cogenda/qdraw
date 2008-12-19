/****************************************************************************
** $Id: rs_actiondrawlinehorvert.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actiondrawlinehorvert.h"
#include "rs_snapper.h"



RS_ActionDrawLineHorVert::RS_ActionDrawLineHorVert(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw horizontal/vertical lines",
                           container, graphicView) {
    reset();
    RS_DEBUG->print("RS_ActionDrawLineHorVert::constructor");
}



RS_ActionDrawLineHorVert::~RS_ActionDrawLineHorVert() {}


QAction* RS_ActionDrawLineHorVert::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("hor./vert. line"),
                                  tr("H&orizontal / Vertical"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Draw horizontal/vertical lines"));
    return action;
}

void RS_ActionDrawLineHorVert::reset() {
    data = RS_LineData(RS_Vector(false),
                       RS_Vector(false));
}



void RS_ActionDrawLineHorVert::init(int status) {
    RS_PreviewActionInterface::init(status);

    reset();
    RS_DEBUG->print("RS_ActionDrawLineHorVert::init");
}



void RS_ActionDrawLineHorVert::trigger() {
    RS_PreviewActionInterface::trigger();

    RS_Line* line = new RS_Line(container, data);
    line->setLayerToActive();
    line->setPenToActive();
    container->addEntity(line);

    // upd. undo list:
    if (document!=NULL) {
        document->startUndoCycle();
        document->addUndoable(line);
        document->endUndoCycle();
    }

    deleteSnapper();
    graphicView->moveRelativeZero(RS_Vector(0.0,0.0));
    graphicView->drawEntity(line);
    graphicView->moveRelativeZero(line->getMiddlepoint());
    RS_DEBUG->print("RS_ActionDrawLineHorVert::trigger():"
                    " line added: %d", line->getId());

}



void RS_ActionDrawLineHorVert::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawLineHorVert::mouseMoveEvent begin");

    RS_Vector mouse = snapPoint(e);
    if (getStatus()==SetEndpoint && p1.valid) {
        RS_Vector p2x = RS_Vector(mouse.x, p1.y);
        RS_Vector p2y = RS_Vector(p1.x, mouse.y);
        if (mouse.distanceTo(p2y) > mouse.distanceTo(p2x))
            p2 = p2x;
        else
            p2 = p2y;
        deletePreview();
        clearPreview();
        data = RS_LineData(p1, p2);
        preview->addEntity(new RS_Line(preview, data));
        drawPreview();
    }

    RS_DEBUG->print("RS_ActionDrawLineHorVert::mouseMoveEvent end");
}



void RS_ActionDrawLineHorVert::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_Vector mouse = snapPoint(e);

        switch (getStatus()) {
        case SetStartpoint:
            p1 = mouse;
            setStatus(SetEndpoint);
            break;

        case SetEndpoint:
            p2 = mouse;
            trigger();
            setStatus(SetStartpoint);
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionDrawLineHorVert::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetStartpoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify first point"),
                                            tr("Cancel"));
        break;
    case SetEndpoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify second point"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}


void RS_ActionDrawLineHorVert::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}


void RS_ActionDrawLineHorVert::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
    }
}

// EOF
