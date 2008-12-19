/****************************************************************************
** $Id: rs_actionselectintersected.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actionselectintersected.h"

#include "rs.h"
#include "rs_snapper.h"
#include "rs_selection.h"



/**
 * Constructor.
 *
 * @param select true: select window. false: deselect window
 */
RS_ActionSelectIntersected::RS_ActionSelectIntersected(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView,
    bool select)
        : RS_PreviewActionInterface("Select Intersected",
                            container, graphicView) {

    this->select = select;
}


QAction* RS_ActionSelectIntersected::createGUIAction(RS2::ActionType type, QObject* /*parent*/) {
    QAction* action;
    if (type==RS2::ActionSelectIntersected) {
        action = new QAction(tr("Select Intersected Entities"), tr("In&tersected Entities"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Selects all entities intersected by a line"));
    } else {
        action = new QAction(tr("Deselect Intersected Entities"),
                             tr("Deselect Inte&rsected Entities"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Deselects all entities intersected by a line"));
    }
    return action;
}


void RS_ActionSelectIntersected::init(int status) {
    RS_PreviewActionInterface::init(status);

    v1 = v2 = RS_Vector(false);
    snapMode = RS2::SnapFree;
    snapRes = RS2::RestrictNothing;
}



void RS_ActionSelectIntersected::trigger() {
    RS_PreviewActionInterface::trigger();

    if (v1.valid && v2.valid) {
        if (graphicView->toGuiDX(v1.distanceTo(v2))>10) {
            deleteSnapper();

            RS_Selection s(*container, graphicView);
            s.selectIntersected(v1, v2, select);

            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
            }

            init();
        }
    }
}



void RS_ActionSelectIntersected::mouseMoveEvent(RS_MouseEvent* e) {
    if (getStatus()==SetPoint2 && v1.valid) {
        v2 = snapPoint(e);
        deletePreview();
        clearPreview();
        preview->addEntity(new RS_Line(preview,
                                       RS_LineData(RS_Vector(v1.x, v1.y),
                                                   RS_Vector(v2.x, v2.y))));
        drawPreview();
    }
}



void RS_ActionSelectIntersected::mousePressEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case SetPoint1:
            v1 = snapPoint(e);
            setStatus(SetPoint2);
            break;

        default:
            break;
        }
    }

    RS_DEBUG->print("RS_ActionSelectIntersected::mousePressEvent(): %f %f",
                    v1.x, v1.y);
}



void RS_ActionSelectIntersected::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionSelectIntersected::mouseReleaseEvent()");
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        if (getStatus()==SetPoint2) {
            deletePreview();
        }
        deleteSnapper();
        init(getStatus()-1);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        if (getStatus()==SetPoint2) {
            v2 = snapPoint(e);
            trigger();
        }
    }
}



void RS_ActionSelectIntersected::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetPoint1:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Choose first point of intersection line"), tr("Cancel"));
            break;
        case SetPoint2:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Choose second point of intersection line"), tr("Back"));
            break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionSelectIntersected::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::SelectCursor);
}



void RS_ActionSelectIntersected::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            //RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSelect);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSelect);
        }
    }
}

// EOF
