/****************************************************************************
** $Id: rs_actionmodifybevel.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONMODIFYBEVEL_H
#define RS_ACTIONMODIFYBEVEL_H

#include "rs_previewactioninterface.h"
#include "rs_modification.h"


/**
 * This action class can handle user events to bevel corners.
 *
 * @author Andrew Mustun
 */
class RS_ActionModifyBevel : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetEntity1,      /**< Choosing the 1st entity. */
        SetEntity2,      /**< Choosing the 2nd entity. */
		SetLength1,      /**< Setting length 1 in command line. */
		SetLength2       /**< Setting length 2 in command line. */
		//SetTrim             /**< Setting trim flag in command line. */
    };

public:
    RS_ActionModifyBevel(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionModifyBevel() {}
	
	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

	virtual RS2::ActionType rtti() {
		return RS2::ActionModifyBevel;
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

	void setLength1(double l1) {
		data.length1 = l1;
	}

	double getLength1() {
		return data.length1;
	}
	
	void setLength2(double l2) {
		data.length2 = l2;
	}

	double getLength2() {
		return data.length2;
	}

	void setTrim(bool t) {
		data.trim = t;
	}

	bool isTrimOn() {
		return data.trim;
	}

private:
    RS_Entity* entity1;
	RS_Vector coord1;
    RS_Entity* entity2;
	RS_Vector coord2;
	RS_BevelData data;
	/** Last status before entering angle. */
	Status lastStatus;
};

#endif
