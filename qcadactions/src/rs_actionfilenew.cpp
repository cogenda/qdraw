/****************************************************************************
** $Id: rs_actionfilenew.cpp 1090 2004-03-16 10:02:59Z js $
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

#include "rs_actionfilenew.h"

#include "rs_graphic.h"



RS_ActionFileNew::RS_ActionFileNew(RS_EntityContainer& container,
                                   RS_GraphicView& graphicView)
        :RS_ActionInterface("File New", container, graphicView) {}


QAction* RS_ActionFileNew::createGUIAction(RS2::ActionType /*type*/, QObject* parent) {
    QAction* action = new QAction(tr("New Drawing"), QPixmap::fromMimeSource("filenew.png"),
                         tr("&New"), CTRL+Key_N, parent);
    action->setStatusTip(tr("Creates a new drawing"));
    return action;
}


void RS_ActionFileNew::trigger() {
    /*
    // Not supported currently
    RS_DEBUG->print("RS_ActionFileNew::trigger");

    RS_String fileName; //= RS_DIALOGFACTORY->requestFileNewDialog();
    if (graphic!=NULL && !fileName.isEmpty()) {
        graphic->open(fileName, );
}
    */
    finish();
}


void RS_ActionFileNew::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
