/****************************************************************************
** $Id: rs_actioneditpaste.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actioneditpaste.h"

#include "rs_clipboard.h"
#include "rs_modification.h"

/**
 * Constructor.
 *
 * @param undo true for undo and false for redo.
 */
RS_ActionEditPaste::RS_ActionEditPaste( RS_EntityContainer& container,
                                        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Edit Paste",
                           container, graphicView) {}



RS_ActionEditPaste::~RS_ActionEditPaste() {}


QAction* RS_ActionEditPaste::createGUIAction(RS2::ActionType /*type*/, QObject* parent) {
    //icon = QPixmap(editpaste_xpm);
    QAction* action = new QAction(tr("Paste"),
                                  QPixmap::fromMimeSource("editpaste2.png"), tr("&Paste"),
                                  CTRL+Key_V, parent);
    action->setStatusTip(tr("Pastes the clipboard contents"));
    return action;
}

void RS_ActionEditPaste::init(int status) {
    RS_PreviewActionInterface::init(status);
    //trigger();
}



void RS_ActionEditPaste::trigger() {
    deleteSnapper();
    deletePreview();
    clearPreview();

    RS_Modification m(*container, graphicView);
    m.paste(RS_PasteData(targetPoint, 1.0, 0.0, false, ""));
    //std::cout << *RS_Clipboard::instance();

    graphicView->redraw();
    finish();
}


void RS_ActionEditPaste::mouseMoveEvent(RS_MouseEvent* e) {
    switch (getStatus()) {
    case SetTargetPoint:
        targetPoint = snapPoint(e);

        deletePreview();
        clearPreview();
        preview->addAllFrom(*RS_CLIPBOARD->getGraphic());
        preview->move(targetPoint);

		if (graphic!=NULL) {
			RS2::Unit sourceUnit = RS_CLIPBOARD->getGraphic()->getUnit();
			RS2::Unit targetUnit = graphic->getUnit();
			double f = RS_Units::convert(1.0, sourceUnit, targetUnit);
        	preview->scale(targetPoint, RS_Vector(f,f));
		}
        drawPreview();
        break;

    default:
        break;
    }
}



void RS_ActionEditPaste::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionEditPaste::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    targetPoint = e->getCoordinate();
    trigger();
}



void RS_ActionEditPaste::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetTargetPoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Set reference point"),
                                            tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionEditPaste::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionEditPaste::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}


// EOF
