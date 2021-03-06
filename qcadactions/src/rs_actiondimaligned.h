/****************************************************************************
** $Id: rs_actiondimaligned.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONDIMALIGNED_H
#define RS_ACTIONDIMALIGNED_H

#include "rs_actiondimension.h"
#include "rs_dimaligned.h"

/**
 * This action class can handle user events to draw 
 * aligned dimensions.
 *
 * @author Andrew Mustun
 */
class RS_ActionDimAligned : public RS_ActionDimension {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetExtPoint1,    /**< Setting the 1st ext point.  */
        SetExtPoint2,    /**< Setting the 2nd ext point. */
        SetDefPoint,     /**< Setting the common def point */
		SetText          /**< Setting the text label in command line */
    };

public:
    RS_ActionDimAligned(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionDimAligned();

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);
	
	virtual RS2::ActionType rtti() {
		return RS2::ActionDimAligned;
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

protected:
    /**
     * Aligned dimension data.
     */
    RS_DimAlignedData edata;

	/** Last status before entering text. */
	Status lastStatus;
}
;

#endif
