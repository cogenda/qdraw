/****************************************************************************
** $Id: rs_actionmodifymoverotate.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionmodifymoverotate.h"

#include "rs_snapper.h"



RS_ActionModifyMoveRotate::RS_ActionModifyMoveRotate(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Move and Rotate Entities",
                           container, graphicView) {
}

QAction* RS_ActionModifyMoveRotate::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Move and Rotate"), tr("M&ove and Rotate"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Move and Rotate Entities"));
    return action;
}

void RS_ActionModifyMoveRotate::init(int status) {
    RS_ActionInterface::init(status);
}



void RS_ActionModifyMoveRotate::trigger() {

    RS_DEBUG->print("RS_ActionModifyMoveRotate::trigger()");

    RS_Modification m(*container, graphicView);
    m.moveRotate(data);

    finish();

    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionModifyMoveRotate::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyMoveRotate::mouseMoveEvent begin");

    if (getStatus()==SetReferencePoint ||
            getStatus()==SetTargetPoint) {

        RS_Vector mouse = snapPoint(e);
        switch (getStatus()) {
        case SetReferencePoint:
            data.referencePoint = mouse;
            break;

        case SetTargetPoint:
            if (data.referencePoint.valid) {
                targetPoint = mouse;
                data.offset = targetPoint-data.referencePoint;

                deletePreview();
                clearPreview();
                preview->addSelectionFrom(*container);
                preview->rotate(data.referencePoint, data.angle);
                preview->move(data.offset);
                drawPreview();
            }
            break;

        default:
            break;
        }
    }

    RS_DEBUG->print("RS_ActionModifyMoveRotate::mouseMoveEvent end");
}



void RS_ActionModifyMoveRotate::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionModifyMoveRotate::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector pos = e->getCoordinate();

    switch (getStatus()) {
    case SetReferencePoint:
        data.referencePoint = pos;
        setStatus(SetTargetPoint);
        break;

    case SetTargetPoint:
        targetPoint = pos;

        setStatus(ShowDialog);
        data.offset = targetPoint - data.referencePoint;
        if (RS_DIALOGFACTORY->requestMoveRotateDialog(data)) {
            trigger();
            //finish();
        }
        break;

    default:
        break;
    }
}


void RS_ActionModifyMoveRotate::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetReferencePoint:
    case SetTargetPoint:
        if (c==checkCommand("angle", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetAngle);
        }
        break;

    case SetAngle: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.angle = RS_Math::deg2rad(a);
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;
    }
}



RS_StringList RS_ActionModifyMoveRotate::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetReferencePoint:
    case SetTargetPoint:
        cmd += command("angle");
        break;

    default:
        break;
    }

    return cmd;
}



void RS_ActionModifyMoveRotate::showOptions() {
    //std::cout << "RS_ActionModifyMoveRotate::showOptions()\n";

    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}



void RS_ActionModifyMoveRotate::hideOptions() {
    //std::cout << "RS_ActionModifyMoveRotate::hideOptions()\n";

    RS_ActionInterface::hideOptions();

    RS_DIALOGFACTORY->requestOptions(this, false);
}


void RS_ActionModifyMoveRotate::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetReferencePoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Cancel"));
        break;
    case SetTargetPoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify target point"),
                                            tr("Back"));
        break;
    case SetAngle:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter rotation angle:"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyMoveRotate::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyMoveRotate::updateToolBar() {
    switch (getStatus()) {
    case SetReferencePoint:
    case SetTargetPoint:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
        break;
    }
}


// EOF
