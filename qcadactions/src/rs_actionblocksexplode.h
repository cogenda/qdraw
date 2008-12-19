/****************************************************************************
** $Id: rs_actionblocksexplode.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONBLOCKSEXPLODE_H
#define RS_ACTIONBLOCKSEXPLODE_H

#include "rs_previewactioninterface.h"
#include "rs_insert.h"

/**
 * This action class can handle user events for exploding blocks and
 * other entity containers into single entities.
 *
 * @author Andrew Mustun
 */
class RS_ActionBlocksExplode : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    RS_ActionBlocksExplode(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionBlocksExplode();

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);
	
	virtual RS2::ActionType rtti() {
		return RS2::ActionBlocksExplode;
	}

    virtual void init(int status=0);

    virtual void trigger();
};

#endif
