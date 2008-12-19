/****************************************************************************
** $Id: rs_actionselect.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionselect.h"

#include "rs_snapper.h"
#include "rs_actionselectsingle.h"


RS_ActionSelect::RS_ActionSelect(RS_EntityContainer& container,
                                 RS_GraphicView& graphicView,
                                 RS2::ActionType nextAction)
        :RS_ActionInterface("Select Entities", container, graphicView) {

    this->nextAction = nextAction;
}



void RS_ActionSelect::init(int status) {
	RS_ActionInterface::init(status);
	graphicView->setCurrentAction(
		new RS_ActionSelectSingle(*container, *graphicView));
}



void RS_ActionSelect::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    }
}



void RS_ActionSelect::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            RS_DIALOGFACTORY->requestToolBarSelect(this, nextAction);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
        }
    }
}



// EOF
