/****************************************************************************
** $Id: rs_actiondrawcircle2p.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actiondrawcircle2p.h"
#include "rs_snapper.h"



RS_ActionDrawCircle2P::RS_ActionDrawCircle2P(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw circles",
                           container, graphicView) {
    reset();
}



RS_ActionDrawCircle2P::~RS_ActionDrawCircle2P() {}


QAction* RS_ActionDrawCircle2P::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Circle: 2 Points"), tr("2 Points"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Draw circles with 2 points"));

    return action;
}


void RS_ActionDrawCircle2P::reset() {
    data.reset();
    point1 = RS_Vector(false);
    point2 = RS_Vector(false);
}



void RS_ActionDrawCircle2P::init(int status) {
    RS_PreviewActionInterface::init(status);

    reset();
}



void RS_ActionDrawCircle2P::trigger() {
    RS_PreviewActionInterface::trigger();

    preparePreview();
    if (data.isValid()) {
        RS_Circle* circle = new RS_Circle(container,
                                          data);
        circle->setLayerToActive();
        circle->setPenToActive();
        container->addEntity(circle);

        // upd. undo list:
        if (document!=NULL) {
            document->startUndoCycle();
            document->addUndoable(circle);
            document->endUndoCycle();
        }

        deleteSnapper();
        RS_Vector rz = graphicView->getRelativeZero();
        graphicView->moveRelativeZero(RS_Vector(0.0,0.0));
        graphicView->drawEntity(circle);
        graphicView->moveRelativeZero(rz);
        drawSnapper();

        setStatus(SetPoint1);
        reset();
    } else {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->requestWarningDialog(tr("Invalid Circle data."));
        }
    }
}



void RS_ActionDrawCircle2P::preparePreview() {
    data.reset();
    if (point1.valid && point2.valid) {
        RS_Circle circle(NULL, data);
        bool suc = circle.createFrom2P(point1, point2);
        if (suc) {
            data = circle.getData();
        }
    }
}


void RS_ActionDrawCircle2P::mouseMoveEvent(RS_MouseEvent* e) {
    RS_Vector mouse = snapPoint(e);
    switch (getStatus()) {
    case SetPoint1:
        point1 = mouse;
        break;

    case SetPoint2:
        point2 = mouse;
        preparePreview();
        if (data.isValid()) {
            RS_Circle* circle = new RS_Circle(preview, data);
            deletePreview();
            clearPreview();
            preview->addEntity(circle);
            drawPreview();
        }
        break;

    default:
        break;
    }
}



void RS_ActionDrawCircle2P::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionDrawCircle2P::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetPoint1:
        point1 = mouse;
        graphicView->moveRelativeZero(mouse);
        setStatus(SetPoint2);
        break;

    case SetPoint2:
        point2 = mouse;
        graphicView->moveRelativeZero(mouse);
        trigger();
        break;

    default:
        break;
    }
}



void RS_ActionDrawCircle2P::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

}



RS_StringList RS_ActionDrawCircle2P::getAvailableCommands() {
    RS_StringList cmd;
    return cmd;
}



void RS_ActionDrawCircle2P::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetPoint1:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify first point"), tr("Cancel"));
            break;
        case SetPoint2:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify second point"), tr("Back"));
            break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionDrawCircle2P::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawCircle2P::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarCircles);
        }
    }
}


// EOF

