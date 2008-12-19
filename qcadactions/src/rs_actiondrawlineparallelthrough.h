/****************************************************************************
** $Id: rs_actiondrawlineparallelthrough.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONDRAWLINEPARALLELTHROUGH_H
#define RS_ACTIONDRAWLINEPARALLELTHROUGH_H

#include "rs_previewactioninterface.h"


/**
 * This action class can handle user events to draw parallel 
 * lines, arcs and circles.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawLineParallelThrough : public RS_PreviewActionInterface {
	Q_OBJECT
private:
    enum Status {
        SetEntity,    /**< Choose original entity. */
		SetPos,       /**< Setting point for this parallel to go through. */
		SetNumber     /**< Setting number in the command line. */
	};
	
public:
    RS_ActionDrawLineParallelThrough(RS_EntityContainer& container,
                              RS_GraphicView& graphicView);
    ~RS_ActionDrawLineParallelThrough() {}

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);
	
	virtual RS2::ActionType rtti() {
		return RS2::ActionDrawLineParallelThrough;
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
    /** Closest parallel. */
    RS_Entity* parallel;
    /** Data of new line */
    RS_LineData data;
	/** Number of parallels. */
	int number;
	/** Coordinate of the mouse. */
	RS_Vector coord;
	/** Original entity. */
	RS_Entity* entity;
	/** Last status before entering length or number. */
	Status lastStatus;
	/**
	 * Commands
	 */
	/*
	RS_String cmdDistance;
	RS_String cmdDistance2;
	RS_String cmdDistance3;
	
	RS_String cmdNumber;
	RS_String cmdNumber2;
	RS_String cmdNumber3;
	*/
};

#endif
