/****************************************************************************
** $Id: rs_actionmodifyexplodetext.cpp 1077 2004-03-03 19:06:45Z andrew $
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

#include "rs_actionmodifyexplodetext.h"

#include "rs_modification.h"

/**
 * Constructor.
 */
RS_ActionModifyExplodeText::RS_ActionModifyExplodeText(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Blocks Explode",
                           container, graphicView) {
}



RS_ActionModifyExplodeText::~RS_ActionModifyExplodeText() {}


QAction* RS_ActionModifyExplodeText::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Explode Text"),
                             tr("&Explode Text into Letters"),
                             QKeySequence(), NULL);
    action->setStatusTip(tr("Explodes Text Entities into single Letters"));
    return action;
}


void RS_ActionModifyExplodeText::init(int status) {
    RS_PreviewActionInterface::init(status);

    trigger();
    finish();
}



void RS_ActionModifyExplodeText::trigger() {
    RS_Modification m(*container, graphicView);
    m.explodeTextIntoLetters();
}


// EOF
