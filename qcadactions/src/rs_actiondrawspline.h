/****************************************************************************
** $Id: rs_actiondrawspline.h 1155 2004-11-18 16:01:06Z andrew $
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

#ifndef RS_ACTIONDRAWSPLINE_H
#define RS_ACTIONDRAWSPLINE_H

#include "rs_previewactioninterface.h"
#include "rs_spline.h"

#include <qaction.h>


/**
 * This action class can handle user events to draw splines.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawSpline : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetStartpoint,   /**< Setting the startpoint.  */
        SetNextPoint      /**< Setting the next point. */
    };

public:
    RS_ActionDrawSpline(RS_EntityContainer& container,
                      RS_GraphicView& graphicView);
    virtual ~RS_ActionDrawSpline();

	virtual RS2::ActionType rtti() {
		return RS2::ActionDrawSpline;
	}

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);
	
    void reset();

    virtual void init(int status=0);
    virtual void trigger();
	
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
	
	virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();
	
	virtual void showOptions();
	virtual void hideOptions();
	
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

	//void close();
	void undo();
	void setDegree(int deg);
	int getDegree();
	void setClosed(bool c);
	bool isClosed();

protected:
    /**
     * Spline data defined so far.
     */
    RS_SplineData data;
	
    /**
     * Polyline entity we're working on.
     */
    RS_Spline* spline;
	
    /**
     * last point.
     */
    //RS_Vector point;

	/**
	 * Start point of the series of nodes. Used for close function.
	 */
	//RS_Vector start;

	/**
	 * Point history (for undo)
	 */
	RS_PtrList<RS_Vector> history;
	
	/**
	 * Bulge history (for undo)
	 */
	//RS_PtrList<double> bHistory;
};

#endif
