/****************************************************************************
** $Id: rs_actionlayersfreezeall.cpp 1092 2004-03-16 18:01:08Z andrew $
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

#include "rs_actionlayersfreezeall.h"

#include "rs_graphic.h"



RS_ActionLayersFreezeAll::RS_ActionLayersFreezeAll(bool freeze,
        RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Freeze all Layers",
                    container, graphicView) {

    this->freeze = freeze;
}

QAction* RS_ActionLayersFreezeAll::createGUIAction(RS2::ActionType type, QObject* /*parent*/) {
    QAction* action=NULL;
	
	if (type==RS2::ActionLayersFreezeAll) {
		action = new QAction(tr("Freeze all"), tr("&Freeze all"),
                                  QKeySequence(), NULL);
    	action->setStatusTip(tr("Freeze all layers"));
	}
	else if (type==RS2::ActionLayersDefreezeAll) {
        action = new QAction(tr("Defreeze all"), tr("&Defreeze all"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Defreeze all layers"));
	}
    return action;
}


void RS_ActionLayersFreezeAll::trigger() {
    RS_DEBUG->print("RS_ActionLayersFreezeAll::trigger");
    if (graphic!=NULL) {
        //RS_Layer* layer = graphic->getActiveLayer();
        graphic->freezeAllLayers(freeze);
    }
    finish();
}



void RS_ActionLayersFreezeAll::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
