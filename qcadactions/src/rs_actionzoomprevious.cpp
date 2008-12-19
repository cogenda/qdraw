/****************************************************************************
** $Id: rs_actionzoomprevious.cpp 1115 2004-04-05 16:47:51Z andrew $
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

#include "rs_actionzoomprevious.h"


/**
 * Constructor.
 */
RS_ActionZoomPrevious::RS_ActionZoomPrevious(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionInterface("Previous zoom", container, graphicView) {

}


QAction* RS_ActionZoomPrevious::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        QAction* action = new QAction(tr("Previous View"), QPixmap::fromMimeSource("zoomprevious.png"),
                             tr("&Previous View"), QKeySequence(), NULL);
        action->setStatusTip(tr("Shows previous view"));
		return action;
}


void RS_ActionZoomPrevious::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}



void RS_ActionZoomPrevious::trigger() {
    graphicView->zoomPrevious();
    finish();
}

// EOF
