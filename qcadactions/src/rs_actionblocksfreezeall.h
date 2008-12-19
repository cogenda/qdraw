/****************************************************************************
** $Id: rs_actionblocksfreezeall.h 1092 2004-03-16 18:01:08Z andrew $
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

#ifndef RS_ACTIONBLOCKSFREEZEALL_H
#define RS_ACTIONBLOCKSFREEZEALL_H

#include "rs_actioninterface.h"


/**
 * This action class can handle user events to edit layers.
 *
 * @author Andrew Mustun
 */
class RS_ActionBlocksFreezeAll : public RS_ActionInterface {
	Q_OBJECT
public:
    RS_ActionBlocksFreezeAll(bool freeze,
                             RS_EntityContainer& container,
                             RS_GraphicView& graphicView);
    ~RS_ActionBlocksFreezeAll() {}

	static QAction* createGUIAction(RS2::ActionType type, QObject* /*parent*/);

    virtual void init(int status=0);
    virtual void trigger();

private:
    bool freeze;
};

#endif
