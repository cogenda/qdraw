/****************************************************************************
** $Id: rs_actiondimradial.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONDIMRADIAL_H
#define RS_ACTIONDIMRADIAL_H

#include "rs_actiondimension.h"
#include "rs_dimradial.h"


/**
 * This action class can handle user events to draw radial dimensions.
 *
 * @author Andrew Mustun
 */
class RS_ActionDimRadial : public RS_ActionDimension {
	Q_OBJECT
private:
    enum Status {
        SetEntity,     /**< Choose entity. */
        SetPos,      /**< Choose point. */
		SetText        /**< Setting text label in the command line. */
    };

public:
    RS_ActionDimRadial(RS_EntityContainer& container,
                              RS_GraphicView& graphicView);
    ~RS_ActionDimRadial() {}

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);
	
	virtual RS2::ActionType rtti() {
		return RS2::ActionDimRadial;
	}

	virtual void reset();
	
    virtual void trigger();
	void preparePreview();
	
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
	
	virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();
	
    virtual void hideOptions();
    virtual void showOptions();

    virtual void updateMouseButtonHints();


private:
    /** Chosen entity (arc / circle) */
    RS_Entity* entity;
	/** Chosen position */
	RS_Vector pos;
    /** Data of new dimension */
    RS_DimRadialData edata;
	/** Last status before entering text. */
	Status lastStatus;
};

#endif
