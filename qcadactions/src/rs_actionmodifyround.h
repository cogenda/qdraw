/****************************************************************************
** $Id: rs_actionmodifyround.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONMODIFYROUND_H
#define RS_ACTIONMODIFYROUND_H

#include "rs_previewactioninterface.h"
#include "rs_modification.h"


/**
 * This action class can handle user events to round corners.
 *
 * @author Andrew Mustun
 */
class RS_ActionModifyRound : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetEntity1,         /**< Choosing the 1st entity. */
        SetEntity2,         /**< Choosing the 2nd entity. */
		SetRadius,          /**< Setting radius in command line. */
		SetTrim             /**< Setting trim flag in command line. */
    };

public:
    RS_ActionModifyRound(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionModifyRound() {}

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);
	
	virtual RS2::ActionType rtti() {
		return RS2::ActionModifyRound;
	}

    virtual void init(int status=0);
    virtual void trigger();

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
	
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();
	
    virtual void hideOptions();
    virtual void showOptions();
	
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();
	
	void setRadius(double r) {
		data.radius = r;
	}

	double getRadius() {
		return data.radius;
	}

	void setTrim(bool t) {
		data.trim = t;
	}

	bool isTrimOn() {
		return data.trim;
	}

private:
	//RS_Vector coord;
	RS_Vector coord1;
    RS_Entity* entity1;
	RS_Vector coord2;
    RS_Entity* entity2;
	RS_RoundData data;
	/** Last status before entering angle. */
	Status lastStatus;
};

#endif
