/****************************************************************************
** $Id: rs_actiondrawcircle.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actiondrawcircle.h"

#include "rs_snapper.h"



RS_ActionDrawCircle::RS_ActionDrawCircle(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw circles",
                           container, graphicView) {
    reset();
}



RS_ActionDrawCircle::~RS_ActionDrawCircle() {}


QAction* RS_ActionDrawCircle::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Circle: Center, Point"),
                                  tr("Center, &Point"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Draw circles with center and point"));
    return action;
}

void RS_ActionDrawCircle::reset() {
    data = RS_CircleData(RS_Vector(false), 0.0);
}



void RS_ActionDrawCircle::init(int status) {
    RS_PreviewActionInterface::init(status);

    reset();
}



void RS_ActionDrawCircle::trigger() {
    RS_PreviewActionInterface::trigger();

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
    graphicView->moveRelativeZero(circle->getCenter());
    drawSnapper();

    setStatus(SetCenter);
    reset();

    RS_DEBUG->print("RS_ActionDrawCircle::trigger(): circle added: %d",
                    circle->getId());
}



void RS_ActionDrawCircle::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawCircle::mouseMoveEvent begin");

    RS_Vector mouse = snapPoint(e);
    switch (getStatus()) {
    case SetCenter:
        data.center = mouse;
        break;

    case SetRadius:
        if (data.center.valid) {
            data.radius = data.center.distanceTo(mouse);
            deletePreview();
            clearPreview();
            preview->addEntity(new RS_Circle(preview,
                                             data));
            drawPreview();
        }
        break;
    }

    RS_DEBUG->print("RS_ActionDrawCircle::mouseMoveEvent end");
}



void RS_ActionDrawCircle::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionDrawCircle::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetCenter:
        data.center = mouse;
        graphicView->moveRelativeZero(mouse);
        setStatus(SetRadius);
        break;

    case SetRadius:
        if (data.center.valid) {
            graphicView->moveRelativeZero(mouse);
            data.radius = data.center.distanceTo(mouse);
            trigger();
        }
        //setStatus(SetCenter);
        break;

    default:
        break;
    }
}



void RS_ActionDrawCircle::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    switch (getStatus()) {

    case SetRadius: {
            bool ok;
            double r = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.radius = r;
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(
                        tr("Not a valid expression"));
                }
            }
            trigger();
            //setStatus(SetCenter);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawCircle::getAvailableCommands() {
    RS_StringList cmd;
    return cmd;
}


void RS_ActionDrawCircle::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetCenter:
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify center"), tr("Cancel"));
        }
        break;
    case SetRadius:
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify radius"), tr("Back"));
        }
        break;
    default:
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->updateMouseWidget("", "");
        }
        break;
    }
}



void RS_ActionDrawCircle::showOptions() {
    RS_ActionInterface::showOptions();
}



void RS_ActionDrawCircle::hideOptions() {
    RS_ActionInterface::hideOptions();
}



void RS_ActionDrawCircle::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawCircle::updateToolBar() {
    if (!isFinished()) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        }
    } else {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarCircles);
        }
    }
}


// EOF

