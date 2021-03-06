/****************************************************************************
** $Id: rs_actionparisdebugcreatecontainer.cpp 1040 2003-11-12 09:58:41Z andrew $
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

#ifndef NO_COMPLEX_ENTITIES

#include "rs_actionparisdebugcreatecontainer.h"

#include "rs_ptrlist.h"

/**
 * Constructor.
 */
RS_ActionPARISDebugCreateContainer::RS_ActionPARISDebugCreateContainer(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        : RS_ActionInterface("rs_actionparischeckcont",
                     container, graphicView) {

    //QMessageBox::about(NULL, "info", "check container");
    RS_Document* theDoc = (RS_Document*) &container;

    if (theDoc->countSelected() < 2) {
        return;
	}

    RS_EntityContainer* con = new RS_EntityContainer(theDoc, true);
    RS_PtrListIterator<RS_Entity> it = theDoc->createIterator();
    RS_Entity* e;

    while ( (e = it.current()) != 0) {
        ++it;
        if (e->isSelected()) {
            con->addEntity(e);
            e->setParent(con);
        }
    }

    theDoc -> addEntity(con);
}



RS_ActionPARISDebugCreateContainer::~RS_ActionPARISDebugCreateContainer() {
}

#endif


// EOF
