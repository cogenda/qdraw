/****************************************************************************
** $Id: rs_actionlayersedit.cpp 1062 2004-01-16 21:51:20Z andrew $
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

#include "rs_actionlayersedit.h"

#include "rs_graphic.h"



RS_ActionLayersEdit::RS_ActionLayersEdit(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Edit Layer", container, graphicView) {}


QAction* RS_ActionLayersEdit::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Edit Layer"), tr("&Edit Layer"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Edit Layer"));
    return action;
}

void RS_ActionLayersEdit::trigger() {
    RS_DEBUG->print("RS_ActionLayersEdit::trigger");

    RS_Layer* layer = NULL;

    if (graphic!=NULL) {
        layer =
            RS_DIALOGFACTORY->requestEditLayerDialog(graphic->getLayerList());

        if (layer!=NULL) {
            graphic->editLayer(graphic->getActiveLayer(), *layer);

            // update updateable entities on the layer that has changed
            for (RS_Entity* e=graphic->firstEntity(RS2::ResolveNone);
                    e!=NULL;
                    e=graphic->nextEntity(RS2::ResolveNone)) {

                RS_Layer* l = e->getLayer();
                if (l!=NULL && l->getName()==layer->getName()) {
                    e->update();
                }
            }
        }
    }
    finish();

    graphicView->redraw();
}



void RS_ActionLayersEdit::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
