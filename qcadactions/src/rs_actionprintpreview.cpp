/****************************************************************************
** $Id: rs_actionprintpreview.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionprintpreview.h"

/**
 * Constructor.
 */
RS_ActionPrintPreview::RS_ActionPrintPreview(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Print Preview",
                    container, graphicView) {
    showOptions();
}



RS_ActionPrintPreview::~RS_ActionPrintPreview() {
}


QAction* RS_ActionPrintPreview::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        QAction* action = new QAction(tr("Print Preview"), QPixmap::fromMimeSource("fileprintpreview.png"), tr("Print Pre&view"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Shows a preview of a print"));
		return action;
}


void RS_ActionPrintPreview::init(int status) {
    RS_ActionInterface::init(status);
    showOptions();
}




void RS_ActionPrintPreview::trigger() {}



void RS_ActionPrintPreview::mouseMoveEvent(RS_MouseEvent* e) {
	switch (getStatus()) {
	case Moving:
		v2 = graphicView->toGraph(e->x(), e->y());
		if (graphic!=NULL) {
			RS_Vector pinsbase = graphic->getPaperInsertionBase();
			
			double scale = graphic->getPaperScale();
			
			graphic->setPaperInsertionBase(pinsbase-v2*scale+v1*scale);
		}
		v1 = v2;
		graphicView->redraw();
		break;

	default:
		break;
	}
}



void RS_ActionPrintPreview::mousePressEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case Neutral:
            v1 = graphicView->toGraph(e->x(), e->y());
            setStatus(Moving);
            break;

        default:
            break;
        }
    }
}


void RS_ActionPrintPreview::mouseReleaseEvent(RS_MouseEvent* e) {
	switch (getStatus()) {
	case Moving:
		setStatus(Neutral);
		break;
		
    default:
        //deleteSnapper();
        RS_DIALOGFACTORY->requestPreviousMenu();
#if QT_VERSION>=0x030000		
        e->accept();
#endif		
        break;
	}
}



void RS_ActionPrintPreview::coordinateEvent(RS_CoordinateEvent* ) {}



void RS_ActionPrintPreview::commandEvent(RS_CommandEvent* ) {}



RS_StringList RS_ActionPrintPreview::getAvailableCommands() {
    RS_StringList cmd;
    return cmd;
}


void RS_ActionPrintPreview::showOptions() {
    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}



void RS_ActionPrintPreview::hideOptions() {
    RS_ActionInterface::hideOptions();

    RS_DIALOGFACTORY->requestOptions(this, false);
}


void RS_ActionPrintPreview::updateMouseButtonHints() {}



void RS_ActionPrintPreview::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::MovingHandCursor);
}



void RS_ActionPrintPreview::updateToolBar() {}


void RS_ActionPrintPreview::center() {
    if (graphic!=NULL) {
        graphic->centerToPage();
        graphicView->redraw();
    }
}


void RS_ActionPrintPreview::fit() {
    if (graphic!=NULL) {
        graphic->fitToPage();
        graphicView->redraw();
    }
}


void RS_ActionPrintPreview::setScale(double f) {
    if (graphic!=NULL) {
		graphic->setPaperScale(f);
    	graphicView->redraw();
	}
}



double RS_ActionPrintPreview::getScale() {
	double ret = 1.0;
    if (graphic!=NULL) {
		ret = graphic->getPaperScale();
	}
	return ret;
}



void RS_ActionPrintPreview::setBlackWhite(bool bw) {
    if (bw) {
        graphicView->setDrawingMode(RS2::ModeBW);
    }
	else {
        graphicView->setDrawingMode(RS2::ModeFull);
	}
	graphicView->redraw();
}


RS2::Unit RS_ActionPrintPreview::getUnit() {
    if (graphic!=NULL) {
		return graphic->getUnit();
	}
	else {
		return RS2::None;
	}
}


// EOF
