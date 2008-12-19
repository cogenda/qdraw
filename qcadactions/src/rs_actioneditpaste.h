/****************************************************************************
** $Id: rs_actioneditpaste.h 1155 2004-11-18 16:01:06Z andrew $
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

#ifndef RS_ACTIONEDITPASTE_H
#define RS_ACTIONEDITPASTE_H

#include "rs_previewactioninterface.h"

/**
 * This action class can handle user events for pasting entities from
 * the clipboard into the current document.
 *
 * @author Andrew Mustun
 */
class RS_ActionEditPaste : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetTargetPoint    /**< Setting the reference point. */
    };

public:
    RS_ActionEditPaste( RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionEditPaste();
	
	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

    virtual void init(int status=0);

    virtual void trigger();

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

    virtual void coordinateEvent(RS_CoordinateEvent* e);

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

protected:
    RS_Vector targetPoint;
};

#endif
