/****************************************************************************
** $Id: rs_actionlockrelativezero.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONLOCKRELATIVEZERO_H
#define RS_ACTIONLOCKRELATIVEZERO_H

#include "rs_previewactioninterface.h"


/**
 * This action class can handle user events to lock or unlock
 * the relative Zero point.
 * It overrides but retains the locking of the relative Zero.
 *
 * @author Ulf Lehnert
 */
class RS_ActionLockRelativeZero : public RS_ActionInterface {
	Q_OBJECT
public:
    RS_ActionLockRelativeZero(RS_EntityContainer& container,
                             RS_GraphicView& graphicView,
							 bool on);
    ~RS_ActionLockRelativeZero() {}

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

    virtual void init(int status=0);
    virtual void trigger();

private:
	bool on;
};

#endif
