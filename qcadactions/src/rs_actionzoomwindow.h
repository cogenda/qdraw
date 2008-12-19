/****************************************************************************
** $Id: rs_actionzoomwindow.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONZOOMWINDOW_H
#define RS_ACTIONZOOMWINDOW_H

#include "rs_previewactioninterface.h"

/**
 * This action class can handle user events to zoom in a window.
 *
 * @author Andrew Mustun
 */
class RS_ActionZoomWindow : public RS_PreviewActionInterface {
    Q_OBJECT
public:
    RS_ActionZoomWindow(RS_EntityContainer& container,
                        RS_GraphicView& graphicView,
                        bool keepAspectRatio=true);

    ~RS_ActionZoomWindow() {}

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

    virtual void init(int status=0);
    virtual void trigger();
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mousePressEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();

protected:
    RS_Vector v1;
    RS_Vector v2;

    bool keepAspectRatio;
};

#endif
