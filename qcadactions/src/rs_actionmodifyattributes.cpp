/****************************************************************************
** $Id: rs_actionmodifyattributes.cpp 1062 2004-01-16 21:51:20Z andrew $
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

#include "rs_actionmodifyattributes.h"

#include "rs_modification.h"
#include "rs_snapper.h"



RS_ActionModifyAttributes::RS_ActionModifyAttributes(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionInterface("Change Attributes",
                    container, graphicView) {}


QAction* RS_ActionModifyAttributes::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Attributes"), tr("&Attributes"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Modify Entity Attributes"));

    return action;
}


void RS_ActionModifyAttributes::init(int status) {
    RS_ActionInterface::init(status);

    trigger();
}



void RS_ActionModifyAttributes::trigger() {

    RS_DEBUG->print("RS_ActionModifyAttributes::trigger()");

    RS_AttributesData data;
    data.pen = RS_Pen();
    data.layer = "0";
    data.changeColor = false;
    data.changeLineType = false;
    data.changeWidth = false;
    data.changeLayer = false;

    if (graphic!=NULL) {
        if (RS_DIALOGFACTORY->requestAttributesDialog(data,
                *graphic->getLayerList())) {
            RS_Modification m(*container, graphicView);
            m.changeAttributes(data);
        }
    }

    finish();
    graphicView->killSelectActions();

    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionModifyAttributes::updateMouseButtonHints() {
    switch (getStatus()) {
        //case Acknowledge:
        //RS_DIALOGFACTORY->updateMouseWidget(tr("Acknowledge"), tr("Cancel"));
        //break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyAttributes::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::DelCursor);
}



void RS_ActionModifyAttributes::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}


// EOF
