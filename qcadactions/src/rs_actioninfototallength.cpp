/****************************************************************************
** $Id: rs_actioninfototallength.cpp 1072 2004-02-18 16:03:20Z andrew $
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

#include "rs_actioninfototallength.h"

#include "rs_actionselectsingle.h"
#include "rs_modification.h"
#include "rs_snapper.h"
#include "rs_point.h"



RS_ActionInfoTotalLength::RS_ActionInfoTotalLength(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Info Total Length",
                    container, graphicView) {}


QAction* RS_ActionInfoTotalLength::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Total length of selected entities"),
                                  tr("&Total length of selected entities"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Measures the total length of all "
                            "selected entities"));

    return action;
}

void RS_ActionInfoTotalLength::init(int status) {
    RS_ActionInterface::init(status);

    trigger();
}



void RS_ActionInfoTotalLength::trigger() {

    RS_DEBUG->print("RS_ActionInfoTotalLength::trigger()");

    double len = 0.0;
    for (RS_Entity* e = container->firstEntity(RS2::ResolveNone);
            e != NULL;
            e = container->nextEntity(RS2::ResolveNone)) {

        if (e->isVisible() && e->isSelected()) {
            double l = e->getLength();
            if (l<0.0) {
                len = -1.0;
                break;
            } else {
                len += l;
            }
        }
    }

    if (len>0.0) {
        RS_DIALOGFACTORY->commandMessage(
            tr("Total Length of selected entities: %1").arg(len));
    } else {
        RS_DIALOGFACTORY->commandMessage(tr("At least one of the selected "
                                            "entities cannot be measured."));
    }

    finish();
}



void RS_ActionInfoTotalLength::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarInfo);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
        }
    }
}


// EOF
