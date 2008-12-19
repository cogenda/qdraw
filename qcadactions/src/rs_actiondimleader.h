/****************************************************************************
** $Id: rs_actiondimleader.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONDIMLEADER_H
#define RS_ACTIONDIMLEADER_H

#include "rs_previewactioninterface.h"
#include "rs_leader.h"

/**
 * This action class can handle user events to draw 
 * leaders (arrows).
 *
 * @author Andrew Mustun
 */
class RS_ActionDimLeader : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetStartpoint,   /**< Setting the startpoint.  */
        SetEndpoint      /**< Setting the endpoint. */
    };

public:
    RS_ActionDimLeader(RS_EntityContainer& container,
                      RS_GraphicView& graphicView);
    ~RS_ActionDimLeader();
	
	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

	virtual RS2::ActionType rtti() {
		return RS2::ActionDimLeader;
	}

    void reset();

    virtual void init(int status=0);
    virtual void trigger();
	
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    virtual void keyPressEvent(RS_KeyEvent*);
	
	virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();
	
	virtual void showOptions();
	virtual void hideOptions();
	
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

protected:
    /**
     * Leader entity.
     */
    RS_Leader* leader;
	/**
	 * Points set so far.
	 */
	RS_PtrList<RS_Vector> points;

};

#endif
