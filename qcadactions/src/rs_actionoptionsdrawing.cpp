/****************************************************************************
** $Id: rs_actionoptionsdrawing.cpp 1090 2004-03-16 10:02:59Z js $
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

#include "rs_actionoptionsdrawing.h"

#include "rs_snapper.h"
#include "rs_graphicview.h"



RS_ActionOptionsDrawing::RS_ActionOptionsDrawing(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Drawing Options",
                    container, graphicView) {
}


QAction* RS_ActionOptionsDrawing::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Drawing"), tr("Current &Drawing Preferences")+"...",
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Settings for the current Drawing"));
    return action;
}


void RS_ActionOptionsDrawing::init(int status) {
    RS_ActionInterface::init(status);

    trigger();
}



void RS_ActionOptionsDrawing::trigger() {
    if (graphic!=NULL) {
        RS_DIALOGFACTORY->requestOptionsDrawingDialog(*graphic);
        RS_DIALOGFACTORY->updateCoordinateWidget(RS_Vector(0.0,0.0),
                RS_Vector(0.0,0.0),
                true);
        graphicView->updateGrid();
        graphicView->redraw();
    }
    finish();
}


// EOF
