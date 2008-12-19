/****************************************************************************
** $Id: rs_actionzoomauto.cpp 1090 2004-03-16 10:02:59Z js $
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

#include "rs_actionzoomauto.h"


/**
 * Constructor.
 *
 * @param keepAspectRatio true: keep same zoom value for x/y.
 *                        false: adjust both x and y individually
 */
RS_ActionZoomAuto::RS_ActionZoomAuto(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView,
                                     bool keepAspectRatio)
        :RS_ActionInterface("Auto zoom", container, graphicView) {

    this->keepAspectRatio = keepAspectRatio;
}


QAction* RS_ActionZoomAuto::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        QAction* action = new QAction(tr("Auto Zoom"), QPixmap::fromMimeSource("zoomauto.png"),
                             tr("&Auto Zoom"), QKeySequence(), NULL);
        action->setStatusTip(tr("Zooms automatic"));
		return action;
}


void RS_ActionZoomAuto::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}



void RS_ActionZoomAuto::trigger() {
    graphicView->zoomAuto(false, keepAspectRatio);
    finish();
}

// EOF
