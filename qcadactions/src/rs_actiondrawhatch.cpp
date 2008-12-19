/****************************************************************************
** $Id: rs_actiondrawhatch.cpp 2381 2005-05-16 17:05:44Z andrew $
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

#include "rs_actiondrawhatch.h"

#include "rs_snapper.h"
#include "rs_graphicview.h"
#include "rs_information.h"



RS_ActionDrawHatch::RS_ActionDrawHatch(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw Hatch",
                           container, graphicView) {

    hatch = NULL;
}


QAction* RS_ActionDrawHatch::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Hatch"), tr("&Hatch"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Draw Hatches and Solid Fills"));
    return action;
}


void RS_ActionDrawHatch::init(int status) {
    RS_ActionInterface::init(status);

    clearPreview();

    RS_Hatch tmp(container, data);
    tmp.setLayerToActive();
    tmp.setPenToActive();
    if (RS_DIALOGFACTORY->requestHatchDialog(&tmp)) {
        data = tmp.getData();
        trigger();
        finish();
        graphicView->redraw();
    } else {
        finish();
    }
}



void RS_ActionDrawHatch::trigger() {

    RS_DEBUG->print("RS_ActionDrawHatch::trigger()");

    //if (pos.valid) {
    //deletePreview();
    deleteSnapper();
	RS_Entity* e;

	// deselect unhatchable entities:
    for (e=container->firstEntity(RS2::ResolveNone); e!=NULL;
            e=container->nextEntity(RS2::ResolveNone)) {
        if (e->isSelected() && 
		    (e->rtti()==RS2::EntityHatch ||
			 e->rtti()==RS2::EntityEllipse || e->rtti()==RS2::EntityPoint ||
			 e->rtti()==RS2::EntityText || 
			 RS_Information::isDimension(e->rtti()))) {
			e->setSelected(false);
        }
    }
    for (e=container->firstEntity(RS2::ResolveAll); e!=NULL;
            e=container->nextEntity(RS2::ResolveAll)) {
        if (e->isSelected() && 
		    (e->rtti()==RS2::EntityHatch ||
			 e->rtti()==RS2::EntityEllipse || e->rtti()==RS2::EntityPoint ||
			 e->rtti()==RS2::EntityText || 
			 RS_Information::isDimension(e->rtti()))) {
			e->setSelected(false);
        }
    }

	// look for selected contours:
    bool haveContour = false;
    for (e=container->firstEntity(RS2::ResolveAll); e!=NULL;
            e=container->nextEntity(RS2::ResolveAll)) {
        if (e->isSelected()) {
            haveContour = true;
        }
    }

    if (!haveContour) {
        std::cerr << "no contour selected\n";
        return;
    }

    hatch = new RS_Hatch(container, data);
    hatch->setLayerToActive();
    hatch->setPenToActive();
    RS_EntityContainer* loop = new RS_EntityContainer(hatch);
    loop->setPen(RS_Pen(RS2::FlagInvalid));

    // add selected contour:
    for (RS_Entity* e=container->firstEntity(RS2::ResolveAll); e!=NULL;
            e=container->nextEntity(RS2::ResolveAll)) {

        if (e->isSelected()) {
            e->setSelected(false);
			// entity is part of a complex entity (spline, polyline, ..):
			if (e->getParent()!=NULL && 
			    (e->getParent()->rtti()==RS2::EntitySpline ||
				 e->getParent()->rtti()==RS2::EntityPolyline)) {
				e->getParent()->setSelected(false);
			}
            RS_Entity* cp = e->clone();
            cp->setPen(RS_Pen(RS2::FlagInvalid));
            cp->reparent(loop);
            loop->addEntity(cp);
        }
    }

    hatch->addEntity(loop);
	if (hatch->validate()) {
		container->addEntity(hatch);

		if (document) {
			document->startUndoCycle();
			document->addUndoable(hatch);
			document->endUndoCycle();
		}
		hatch->update();

		graphicView->drawEntity(hatch);
		
		RS_DIALOGFACTORY->commandMessage(tr("Hatch created successfully."));
	}
	else {
		delete hatch;
		hatch = NULL;
		RS_DIALOGFACTORY->commandMessage(tr("Invalid hatch area. Please check that "
		"the entities chosen form one or more closed contours."));
	}
    //}
}



void RS_ActionDrawHatch::mouseMoveEvent(RS_MouseEvent*) {
    RS_DEBUG->print("RS_ActionDrawHatch::mouseMoveEvent begin");

    /*if (getStatus()==SetPos) {
        RS_Vector mouse = snapPoint(e);
        pos = mouse;


        deletePreview();
        if (hatch!=NULL && !hatch->isVisible()) {
            hatch->setVisible(true);
        }
        offset = RS_Vector(graphicView->toGuiDX(pos.x),
                           -graphicView->toGuiDY(pos.y));
        drawPreview();
}*/

    RS_DEBUG->print("RS_ActionDrawHatch::mouseMoveEvent end");
}



void RS_ActionDrawHatch::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_Vector mouse = snapPoint(e);

        switch (getStatus()) {
        case ShowDialog:
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        //deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionDrawHatch::updateMouseButtonHints() {
    RS_DIALOGFACTORY->updateMouseWidget("", "");
}



void RS_ActionDrawHatch::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawHatch::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
}


// EOF
