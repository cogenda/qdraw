/****************************************************************************
** $Id: rs_actiondrawlinerectangle.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONDRAWLINERECTANGLE_H
#define RS_ACTIONDRAWLINERECTANGLE_H

#include "rs_previewactioninterface.h"
#include "rs_line.h"

/**
 * This action class can handle user events to draw 
 * rectangles with two corners given.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawLineRectangle : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetCorner1,      /**< Setting the 1st corner.  */
        SetCorner2       /**< Setting the 2nd corner. */
    };

public:
    RS_ActionDrawLineRectangle(RS_EntityContainer& container,
                               RS_GraphicView& graphicView);
    ~RS_ActionDrawLineRectangle();

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

    void reset();

    virtual void init(int status=0);
    virtual void trigger();
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
	void preparePreview();
	
	virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

protected:
    /**
     * Line data for the 4 lines.
     */
    RS_LineData data[4];
    /**
     * 1st corner.
     */
    RS_Vector corner1;
    /**
     * 2nd corner.
     */
    RS_Vector corner2;
};

#endif
