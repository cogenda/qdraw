/****************************************************************************
** $Id: rs_actiondrawlinepolygon2.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONDRAWLINEPOLYGON2_H
#define RS_ACTIONDRAWLINEPOLYGON2_H

#include "rs_previewactioninterface.h"


/**
 * This action class can handle user events to draw polygons.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawLinePolygon2 : public RS_PreviewActionInterface {
	Q_OBJECT
private:
    enum Status {
        SetCorner1,    /**< Setting center 1. */
		SetCorner2,    /**< Setting corner 2. */
		SetNumber      /**< Setting number in the command line. */
	};
	
public:
    RS_ActionDrawLinePolygon2(RS_EntityContainer& container,
                              RS_GraphicView& graphicView);
    ~RS_ActionDrawLinePolygon2() {}

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);
	
	virtual RS2::ActionType rtti() {
		return RS2::ActionDrawLinePolygon2;
	}

    virtual void trigger();
	
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    virtual void updateMouseButtonHints();
	
	virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();
	
    virtual void hideOptions();
    virtual void showOptions();

    virtual void updateMouseCursor();
    virtual void updateToolBar();

	int getNumber() {
		return number;
	}

	void setNumber(int n) {
		number = n;
	}

private:
	/** 1st corner */
	RS_Vector corner1;
	/** 2nd corner */
	RS_Vector corner2;
	/** Number of edges. */
	int number;
	/** Last status before entering text. */
	Status lastStatus;
};

#endif
