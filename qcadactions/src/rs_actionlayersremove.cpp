/****************************************************************************
** $Id: rs_actionlayersremove.cpp 1062 2004-01-16 21:51:20Z andrew $
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

#include "rs_actionlayersremove.h"

#include "rs_graphic.h"



RS_ActionLayersRemove::RS_ActionLayersRemove(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Remove Layer", container, graphicView) {}


QAction* RS_ActionLayersRemove::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Remove Layer"), tr("&Remove Layer"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Remove Layer"));
    return action;
}


void RS_ActionLayersRemove::trigger() {
    RS_DEBUG->print("RS_ActionLayersRemove::trigger");

    if (graphic!=NULL) {
        RS_Layer* layer =
            RS_DIALOGFACTORY->requestLayerRemovalDialog(graphic->getLayerList());

        /*
              if (layer!=NULL && layer->getName()!="0") {

                  graphic->startUndoCycle();
                  for (RS_Entity* e=graphic->firstEntity(RS2::ResolveNone);
                          e!=NULL;
                          e=graphic->nextEntity(RS2::ResolveNone)) {

                      if (e->getLayer()!=NULL &&
                              e->getLayer()->getName()==layer->getName()) {

                          e->setUndoState(true);
                          e->setLayer("0");
                          graphic->addUndoable(e);
                      }
                  }


                  graphic->endUndoCycle();
        */

        // Now remove the layer from the layer list:
        graphic->removeLayer(layer);
    }
    finish();
    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionLayersRemove::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
