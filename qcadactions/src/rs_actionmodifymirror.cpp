/****************************************************************************
** $Id: rs_actionmodifymirror.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionmodifymirror.h"

#include "rs_snapper.h"



RS_ActionModifyMirror::RS_ActionModifyMirror(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Mirror Entities",
                           container, graphicView) {}

QAction* RS_ActionModifyMirror::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Mirror"), tr("&Mirror"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Mirror Entities"));
    return action;
}


void RS_ActionModifyMirror::init(int status) {
    RS_ActionInterface::init(status);
}



void RS_ActionModifyMirror::trigger() {

    RS_DEBUG->print("RS_ActionModifyMirror::trigger()");

    RS_Modification m(*container, graphicView);
    m.mirror(data);

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
}



void RS_ActionModifyMirror::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyMirror::mouseMoveEvent begin");

    if (getStatus()==SetAxisPoint1 ||
            getStatus()==SetAxisPoint2) {

        RS_Vector mouse = snapPoint(e);
        switch (getStatus()) {
        case SetAxisPoint1:
            axisPoint1 = mouse;
            break;

        case SetAxisPoint2:
            if (axisPoint1.valid) {
                axisPoint2 = mouse;

                deletePreview();
                clearPreview();
                preview->addSelectionFrom(*container);
                preview->mirror(axisPoint1, axisPoint2);

                preview->addEntity(new RS_Line(preview,
                                               RS_LineData(axisPoint1,
                                                           axisPoint2)));

                drawPreview();
            }
            break;

        default:
            break;
        }
    }

    RS_DEBUG->print("RS_ActionModifyMirror::mouseMoveEvent end");
}



void RS_ActionModifyMirror::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);

    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionModifyMirror::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

        switch (getStatus()) {
        case SetAxisPoint1:
            axisPoint1 = mouse;
            setStatus(SetAxisPoint2);
        	graphicView->moveRelativeZero(mouse);
            break;

        case SetAxisPoint2:
            axisPoint2 = mouse;
            setStatus(ShowDialog);
        	graphicView->moveRelativeZero(mouse);
            if (RS_DIALOGFACTORY!=NULL) {
                if (RS_DIALOGFACTORY->requestMirrorDialog(data)) {
                    data.axisPoint1 = axisPoint1;
                    data.axisPoint2 = axisPoint2;
                    deletePreview();
                    clearPreview();
                    trigger();
                    finish();
                }
            }
            break;

        default:
            break;
        }
}



void RS_ActionModifyMirror::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
            /*case Select:
                RS_DIALOGFACTORY->updateMouseWidget(tr("Pick entities to move"),
                                               tr("Cancel"));
                break;*/
        case SetAxisPoint1:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify first point of mirror line"),
                tr("Cancel"));
            break;
        case SetAxisPoint2:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify second point of mirror line"),
                tr("Back"));
            break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionModifyMirror::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyMirror::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetAxisPoint1:
        case SetAxisPoint2:
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
            break;
        default:
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
            break;
        }
    }
}


// EOF
