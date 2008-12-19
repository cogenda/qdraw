/****************************************************************************
** $Id: rs_actionsetrelativezero.h 1114 2004-04-04 22:49:45Z andrew $
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

#ifndef RS_ACTIONSETRELATIVEZERO_H
#define RS_ACTIONSETRELATIVEZERO_H

#include "rs_previewactioninterface.h"


/**
 * This action class can handle user events to set
 * the relative Zero point.
 * It overrides but retains the locking of the relative Zero.
 *
 * @author Ulf Lehnert
 */
class RS_ActionSetRelativeZero : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    RS_ActionSetRelativeZero(RS_EntityContainer& container,
                             RS_GraphicView& graphicView);
    ~RS_ActionSetRelativeZero() {}

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

	virtual RS2::ActionType rtti() {
		return RS2::ActionSetRelativeZero;
	}

    virtual void trigger();
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
	
	virtual void coordinateEvent(RS_CoordinateEvent* e);

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

private:
    RS_Vector pt;
};

#endif
