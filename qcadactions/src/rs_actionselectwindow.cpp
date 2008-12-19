/****************************************************************************
** $Id: rs_actionselectwindow.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actionselectwindow.h"

#include "rs.h"
#include "rs_snapper.h"
#include "rs_selection.h"



/**
 * Constructor.
 *
 * @param select true: select window. false: deselect window
 */
RS_ActionSelectWindow::RS_ActionSelectWindow(RS_EntityContainer& container,
        RS_GraphicView& graphicView,
        bool select)
        : RS_PreviewActionInterface("Select Window",
                            container, graphicView) {

    this->select = select;
}


QAction* RS_ActionSelectWindow::createGUIAction(RS2::ActionType type, QObject* /*parent*/) {
    QAction* action;

    if (type==RS2::ActionSelectWindow) {
        action = new QAction(tr("Select Window"), tr("Select &Window"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Selects all Entities in a given Window"));
    } else {
        action = new QAction(tr("Deselect Window"), tr("Deselect &Window"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Deselects all Entities"
                                " in a given Window"));
    }
    return action;
}


void RS_ActionSelectWindow::init(int status) {
    RS_PreviewActionInterface::init(status);
    v1 = v2 = RS_Vector(false);
    snapMode = RS2::SnapFree;
    snapRes = RS2::RestrictNothing;
}



void RS_ActionSelectWindow::trigger() {
    RS_PreviewActionInterface::trigger();

    if (v1.valid && v2.valid) {
        if (graphicView->toGuiDX(v1.distanceTo(v2))>10) {
            deleteSnapper();

            bool cross = (v2.y>v1.y);

            RS_Selection s(*container, graphicView);
            s.selectWindow(v1, v2, select, cross);

            RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());

            init();
        }
    }
}



void RS_ActionSelectWindow::mouseMoveEvent(RS_MouseEvent* e) {
    if (getStatus()==SetCorner2 && v1.valid) {
        v2 = snapPoint(e);
        deletePreview();
        clearPreview();
        preview->addEntity(new RS_Line(preview,
                                       RS_LineData(RS_Vector(v1.x, v1.y),
                                                   RS_Vector(v2.x, v1.y))));
        preview->addEntity(new RS_Line(preview,
                                       RS_LineData(RS_Vector(v2.x, v1.y),
                                                   RS_Vector(v2.x, v2.y))));
        preview->addEntity(new RS_Line(preview,
                                       RS_LineData(RS_Vector(v2.x, v2.y),
                                                   RS_Vector(v1.x, v2.y))));
        preview->addEntity(new RS_Line(preview,
                                       RS_LineData(RS_Vector(v1.x, v2.y),
                                                   RS_Vector(v1.x, v1.y))));
        drawPreview();
    }
}



void RS_ActionSelectWindow::mousePressEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case SetCorner1:
            v1 = snapPoint(e);
            setStatus(SetCorner2);
            break;

        default:
            break;
        }
    }

    RS_DEBUG->print("RS_ActionSelectWindow::mousePressEvent(): %f %f",
                    v1.x, v1.y);
}



void RS_ActionSelectWindow::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionSelectWindow::mouseReleaseEvent()");

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        if (getStatus()==SetCorner2) {
            v2 = snapPoint(e);
            trigger();
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        if (getStatus()==SetCorner2) {
            deletePreview();
        }
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionSelectWindow::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetCorner1:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Choose first edge"), tr("Cancel"));
        break;
    case SetCorner2:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Choose second edge"), tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionSelectWindow::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::SelectCursor);
}



void RS_ActionSelectWindow::updateToolBar() {
    if (!isFinished()) {
        //RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSelect);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSelect);
    }
}

// EOF
