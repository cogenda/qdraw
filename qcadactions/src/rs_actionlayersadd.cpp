/****************************************************************************
** $Id: rs_actionlayersadd.cpp 1062 2004-01-16 21:51:20Z andrew $
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

#include "rs_actionlayersadd.h"

#include "rs_graphic.h"



RS_ActionLayersAdd::RS_ActionLayersAdd(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_ActionInterface("Add Layer", container, graphicView) {}


QAction* RS_ActionLayersAdd::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Add Layer"), tr("&Add Layer"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Add Layer"));

    return action;
}

void RS_ActionLayersAdd::trigger() {
    RS_DEBUG->print("add layer");

    if (RS_DIALOGFACTORY!=NULL) {
        if (graphic) {
            RS_Layer* layer = RS_DIALOGFACTORY->requestNewLayerDialog(
                                  graphic->getLayerList());
            if (layer!=NULL)
                graphic->addLayer(layer);
        }
    }
    finish();
}



void RS_ActionLayersAdd::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
