/****************************************************************************
** $Id: rs_actionlayerstogglelock.cpp 1062 2004-01-16 21:51:20Z andrew $
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

#include "rs_actionlayerstogglelock.h"

#include "rs_graphic.h"



RS_ActionLayersToggleLock::RS_ActionLayersToggleLock(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionInterface("Toggle Layer Visibility",
                    container, graphicView) {}


QAction* RS_ActionLayersToggleLock::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Toggle Layer Lock"),
                                  tr("&Toggle Lock"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Toggle Lock"));
    return action;
}

void RS_ActionLayersToggleLock::trigger() {
    RS_DEBUG->print("toggle layer");
    if (graphic!=NULL) {
        RS_Layer* layer = graphic->getActiveLayer();
        if (layer!=NULL) {
            graphic->toggleLayerLock(layer);

            // deselect entities on locked layer:
            if (layer->isLocked()) {
                for (RS_Entity* e=container->firstEntity(); e!=NULL;
                        e=container->nextEntity()) {
                    if (e!=NULL && e->isVisible() && e->getLayer()==layer) {

                        if (graphicView!=NULL) {
                            graphicView->deleteEntity(e);
                        }

                        e->setSelected(false);

                        if (graphicView!=NULL) {
                            graphicView->drawEntity(e);
                        }
                    }
                }
            }
        }

    }
    finish();
}



void RS_ActionLayersToggleLock::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
