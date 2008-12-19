/****************************************************************************
** $Id: rs_actionmodifyscale.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionmodifyscale.h"

#include "rs_snapper.h"



RS_ActionModifyScale::RS_ActionModifyScale(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Scale Entities",
                           container, graphicView) {}


QAction* RS_ActionModifyScale::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Scale"), tr("&Scale"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Scale Entities"));
    return action;
}

void RS_ActionModifyScale::init(int status) {
    RS_ActionInterface::init(status);

}



void RS_ActionModifyScale::trigger() {

    RS_DEBUG->print("RS_ActionModifyScale::trigger()");

    RS_Modification m(*container, graphicView);
    m.scale(data);

    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionModifyScale::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyScale::mouseMoveEvent begin");

    if (getStatus()==SetReferencePoint) {

        RS_Vector mouse = snapPoint(e);
        switch (getStatus()) {
        case SetReferencePoint:
            referencePoint = mouse;
            break;

        default:
            break;
        }
    }

    RS_DEBUG->print("RS_ActionModifyScale::mouseMoveEvent end");
}



void RS_ActionModifyScale::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_Vector mouse = snapPoint(e);

        switch (getStatus()) {
        case SetReferencePoint:
            setStatus(ShowDialog);
            if (RS_DIALOGFACTORY->requestScaleDialog(data)) {
                data.referencePoint = referencePoint;
                trigger();
                finish();
            }
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



void RS_ActionModifyScale::updateMouseButtonHints() {
    switch (getStatus()) {
        /*case Select:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Pick entities to scale"),
                                           tr("Cancel"));
            break;*/
    case SetReferencePoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyScale::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyScale::updateToolBar() {
    switch (getStatus()) {
        /*case Select:
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSelect);
            break;*/
    case SetReferencePoint:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
        break;
    }
}


// EOF
