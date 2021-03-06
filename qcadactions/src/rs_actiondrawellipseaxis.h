/****************************************************************************
** $Id: rs_actiondrawellipseaxis.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONDRAWELLIPSEAXIS_H
#define RS_ACTIONDRAWELLIPSEAXIS_H

#include "rs_previewactioninterface.h"
#include "rs_ellipse.h"

/**
 * This action class can handle user events to draw ellipses
 * with a center point and the endpoints of minor and major axis.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawEllipseAxis : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetCenter,   /**< Settinge the center.  */
        SetMajor,    /**< Setting endpoint of major axis. */
        SetMinor,    /**< Setting minor/major ratio. */
        SetAngle1,   /**< Setting start angle. */
        SetAngle2    /**< Setting end angle. */
    };

public:
    RS_ActionDrawEllipseAxis(RS_EntityContainer& container,
                             RS_GraphicView& graphicView,
                             bool isArc);
    ~RS_ActionDrawEllipseAxis();
	
	static QAction* createGUIAction(RS2::ActionType type, QObject* /*parent*/);

    virtual void init(int status=0);
	
    virtual void trigger();
	
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

	virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

protected:
    /** Center of ellipse */
    RS_Vector center;
    /** Endpoint of major axis */
    RS_Vector major;
    /** Ratio major / minor */
    double ratio;
    /** Start angle */
    double angle1;
    /** End angle */
    double angle2;
    /** Do we produce an arc (true) or full ellipse (false) */
    bool isArc;
};

#endif
