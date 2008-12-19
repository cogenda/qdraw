/****************************************************************************
** $Id: rs_actiondrawcirclecr.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actiondrawcirclecr.h"
#include "rs_snapper.h"

/**
 * Constructor.
 */
RS_ActionDrawCircleCR::RS_ActionDrawCircleCR(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw circles CR",
                           container, graphicView) {

    reset();
}



RS_ActionDrawCircleCR::~RS_ActionDrawCircleCR() {}


QAction* RS_ActionDrawCircleCR::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Circle: Center, Radius"),
                                  tr("Center, &Radius"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Draw circles with center and radius"));
    return action;
}


void RS_ActionDrawCircleCR::reset() {
    data = RS_CircleData(RS_Vector(false), 0.0);
}



void RS_ActionDrawCircleCR::init(int status) {
    RS_PreviewActionInterface::init(status);
}



void RS_ActionDrawCircleCR::trigger() {
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

    RS_DEBUG->print("RS_ActionDrawCircleCR::trigger(): circle added: %d",
                    circle->getId());
}



void RS_ActionDrawCircleCR::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawCircleCR::mouseMoveEvent begin");

    RS_Vector mouse = snapPoint(e);
    switch (getStatus()) {
    case SetCenter:
        data.center = mouse;
        deletePreview();
        clearPreview();
        preview->addEntity(new RS_Circle(preview,
                                         data));
        drawPreview();
        break;
    }

    RS_DEBUG->print("RS_ActionDrawCircleCR::mouseMoveEvent end");
}



void RS_ActionDrawCircleCR::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionDrawCircleCR::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetCenter:
        data.center = mouse;
        trigger();
        break;

    default:
        break;
    }
}



void RS_ActionDrawCircleCR::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetCenter:
        if (checkCommand("radius", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            setStatus(SetRadius);
        }
        break;

    case SetRadius: {
            bool ok;
            double r = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.radius = r;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(SetCenter);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawCircleCR::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetCenter:
        cmd += command("radius");
        break;
    default:
        break;
    }

    return cmd;
}

void RS_ActionDrawCircleCR::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetCenter:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify circle center"),
                                            tr("Cancel"));
        break;
    case SetRadius:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify circle radius"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDrawCircleCR::showOptions() {
    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}



void RS_ActionDrawCircleCR::hideOptions() {
    RS_ActionInterface::hideOptions();

    RS_DIALOGFACTORY->requestOptions(this, false);
}



void RS_ActionDrawCircleCR::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawCircleCR::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarCircles);
    }
}


// EOF

