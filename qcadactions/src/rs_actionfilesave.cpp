/****************************************************************************
** $Id: rs_actionfilesave.cpp 1096 2004-03-17 11:54:02Z andrew $
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

#include "rs_actionfilesave.h"

#include "rs_graphic.h"



RS_ActionFileSave::RS_ActionFileSave(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionInterface("File Save", container, graphicView) {}


QAction* RS_ActionFileSave::createGUIAction(RS2::ActionType /*type*/, QObject* parent) {
    //icon = QPixmap(filesave_xpm);
    QAction* action = new QAction(tr("Save Drawing"), 
								  QPixmap::fromMimeSource("filesave2.png"),
                                  tr("&Save"), CTRL+Key_S, parent);
    action->setStatusTip(tr("Saves the current drawing"));
    return action;
}


void RS_ActionFileSave::trigger() {
    RS_DEBUG->print("RS_ActionFileSave::trigger");

    if (graphic!=NULL) {
        graphic->save();
    }
    finish();
}



void RS_ActionFileSave::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
