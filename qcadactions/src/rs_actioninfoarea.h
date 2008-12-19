/****************************************************************************
** $Id: rs_actioninfoarea.h 1133 2004-07-11 20:42:18Z andrew $
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

#ifndef RS_ACTIONINFOAREA_H
#define RS_ACTIONINFOAREA_H

#include "rs_previewactioninterface.h"
#include "rs_infoarea.h"


/**
 * This action class can handle user events to measure distances between 
 * two points.
 *
 * @author Andrew Mustun
 */
class RS_ActionInfoArea : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetFirstPoint,    /**< Setting the 1st point of the polygon. */
        SetNextPoint      /**< Setting a next point. */
    };

public:
    RS_ActionInfoArea(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionInfoArea() {}

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

    virtual void init(int status=0);
    virtual void trigger();

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
	
	virtual void coordinateEvent(RS_CoordinateEvent* e);
	
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

private:
    RS_Vector point1;
    RS_Vector prev;
	RS_Entity* currentLine;
	RS_Entity* closingLine;
    //RS_Vector point2;
	RS_InfoArea ia;
};

#endif
