/****************************************************************************
** $Id: rs_actiondrawlineangle.h 1092 2004-03-16 18:01:08Z andrew $
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

#ifndef RS_ACTIONDRAWLINEANGLE_H
#define RS_ACTIONDRAWLINEANGLE_H

#include "rs_previewactioninterface.h"
#include "rs_line.h"

/**
 * This action class can handle user events to draw 
 * simple lines at a gien angle.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawLineAngle : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetPos,       /**< Setting the position.  */
		SetAngle,     /**< Setting angle in the command line. */
		SetLength     /**< Setting length in the command line. */
    };
	
    RS_ActionDrawLineAngle(RS_EntityContainer& container,
                           RS_GraphicView& graphicView,
                           double angle=0.0,
                           bool fixedAngle=false);
    ~RS_ActionDrawLineAngle();

	static QAction* createGUIAction(RS2::ActionType type, QObject* /*parent*/);
	
	virtual RS2::ActionType rtti() {
		return RS2::ActionDrawLineAngle;
	}

    void reset();

    virtual void init(int status=0);
	
    virtual void trigger();

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
	void preparePreview();
	
	virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();
	
    virtual void hideOptions();
    virtual void showOptions();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

	void setSnapPoint(int sp) {
		snpPoint = sp;
	}

	int getSnapPoint() {
		return snpPoint;
	}

	void setAngle(double a) {
		angle = a;
	}

	double getAngle() {
		return angle;
	}

	void setLength(double l) {
		length = l;
	}

	double getLength() {
		return length;
	}

	bool hasFixedAngle() {
		return fixedAngle;
	}

protected:
    /**
     * Line data defined so far.
     */
    RS_LineData data;
	/**
	 * Position.
	 */
	RS_Vector pos;
    /**
     * Line angle.
     */
    double angle;
	/**
	 * Line length.
	 */
	double length;
    /**
     * Is the angle fixed?
     */
    bool fixedAngle;
    /**
     * Snap point (start, middle, end).
     */
    int snpPoint;
};

#endif

