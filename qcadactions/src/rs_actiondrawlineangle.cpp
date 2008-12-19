/****************************************************************************
** $Id: rs_actiondrawlineangle.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actiondrawlineangle.h"
#include "rs_snapper.h"



RS_ActionDrawLineAngle::RS_ActionDrawLineAngle(RS_EntityContainer& container,
        RS_GraphicView& graphicView,
        double angle,
        bool fixedAngle)
        :RS_PreviewActionInterface("Draw lines with given angle",
                           container, graphicView) {

    this->angle = angle;
    length = 1.0;
    snpPoint = 0;
    this->fixedAngle = fixedAngle;
    pos = RS_Vector(false);
    reset();
}



RS_ActionDrawLineAngle::~RS_ActionDrawLineAngle() {}


QAction* RS_ActionDrawLineAngle::createGUIAction(RS2::ActionType type, QObject* /*parent*/) {
    QAction* action=NULL;
	
	if (type==RS2::ActionDrawLineAngle) {
		action = new QAction(tr("Line: Angle"), tr("&Angle"),
                                  QKeySequence(), NULL);
	    action->setStatusTip(tr("Draw lines with a given angle"));
	}
	else if (type==RS2::ActionDrawLineHorizontal) {
        action = new QAction(tr("Line: Horizontal"), tr("&Horizontal"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Draw horizontal lines"));
	}
	else if (type==RS2::ActionDrawLineVertical) {
        action = new QAction(tr("hor./vert. line"), tr("H&orizontal / Vertical"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Draw horizontal/vertical lines"));
	}
    return action;
}

void RS_ActionDrawLineAngle::reset() {
    data = RS_LineData(RS_Vector(false),
                       RS_Vector(false));
}



void RS_ActionDrawLineAngle::init(int status) {
    RS_PreviewActionInterface::init(status);

    reset();
}



void RS_ActionDrawLineAngle::trigger() {
    RS_PreviewActionInterface::trigger();

    preparePreview();
    RS_Line* line = new RS_Line(container,
                                data);
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
    graphicView->moveRelativeZero(data.startpoint);
    RS_DEBUG->print("RS_ActionDrawLineAngle::trigger(): line added: %d",
                    line->getId());
}



void RS_ActionDrawLineAngle::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawLineAngle::mouseMoveEvent begin");

    if (getStatus()==SetPos) {
        pos = snapPoint(e);
        deletePreview();
        clearPreview();
        preparePreview();
        preview->addEntity(new RS_Line(preview,
                                       data));
        drawPreview();
    }

    RS_DEBUG->print("RS_ActionDrawLineAngle::mouseMoveEvent end");
}



void RS_ActionDrawLineAngle::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        if (getStatus()==SetPos) {
            RS_CoordinateEvent ce(snapPoint(e));
            coordinateEvent(&ce);
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}


void RS_ActionDrawLineAngle::preparePreview() {
    RS_Vector p1, p2;
    // End:
    if (snpPoint == 2) {
        p2.setPolar(length * -1, angle);
    } else {
        p2.setPolar(length, angle);
    }

    // Middle:
    if (snpPoint == 1) {
        p1 = pos - (p2 / 2);
    } else {
        p1 = pos;
    }

    p2 += p1;
    data = RS_LineData(p1, p2);
}


void RS_ActionDrawLineAngle::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    switch (getStatus()) {
    case SetPos:
        pos = e->getCoordinate();
        trigger();
        break;

    default:
        break;
    }
}



void RS_ActionDrawLineAngle::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetPos:
        if (!fixedAngle && checkCommand("angle", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            setStatus(SetAngle);
        } else if (checkCommand("length", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            setStatus(SetLength);
        }
        break;

    case SetAngle: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                angle = RS_Math::deg2rad(a);
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(SetPos);
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
            setStatus(SetPos);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawLineAngle::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetPos:
        if (!fixedAngle) {
            cmd += command("angle");
        }
        cmd += command("length");
        break;
    default:
        break;
    }

    return cmd;
}


void RS_ActionDrawLineAngle::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetPos:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify position"),
                                            tr("Cancel"));
        break;

    case SetAngle:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter angle:"), tr("Back"));
        break;

    case SetLength:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter length:"), tr("Back"));
        break;

    default:
        break;
    }
}



void RS_ActionDrawLineAngle::showOptions() {
    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}



void RS_ActionDrawLineAngle::hideOptions() {
    RS_ActionInterface::hideOptions();

    RS_DIALOGFACTORY->requestOptions(this, false);
}



void RS_ActionDrawLineAngle::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawLineAngle::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
    }
}

// EOF
