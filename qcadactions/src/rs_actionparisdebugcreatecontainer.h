/****************************************************************************
** $Id: rs_actionparisdebugcreatecontainer.h 1040 2003-11-12 09:58:41Z andrew $
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

#ifndef RS_ACTIONPARISDEBUGCREATECONTAINER_H
#define RS_ACTIONPARISDEBUGCREATECONTAINER_H

#include "rs_actioninterface.h"

/**
 * Creates an entity group without using blocks.
 *
 * @author Tobias Schulze, shhinfo.de
 */
class RS_ActionPARISDebugCreateContainer : public RS_ActionInterface {
	Q_OBJECT
public:
    RS_ActionPARISDebugCreateContainer(RS_EntityContainer& container,
                         RS_GraphicView& graphicView);
    virtual ~RS_ActionPARISDebugCreateContainer();
	
	virtual RS2::ActionType rtti() {
		return RS2::ActionPARISDebugCreateContainer;
	}
};

#endif
#endif
