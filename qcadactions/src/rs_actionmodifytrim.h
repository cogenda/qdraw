/****************************************************************************
** $Id: rs_actionmodifytrim.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONMODIFYTRIM_H
#define RS_ACTIONMODIFYTRIM_H

#include "rs_previewactioninterface.h"
#include "rs_modification.h"


/**
 * This action class can handle user events to trim entities.
 *
 * @author Andrew Mustun
 */
class RS_ActionModifyTrim : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        ChooseLimitEntity,     /**< Choosing the limiting entity. */
        ChooseTrimEntity       /**< Choosing the entity to trim. */
    };

public:
    RS_ActionModifyTrim(RS_EntityContainer& container,
                        RS_GraphicView& graphicView,
						bool both=false);
    ~RS_ActionModifyTrim() {}
	
	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

    virtual void init(int status=0);
    virtual void trigger();
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

private:
    RS_Entity* limitEntity;
	RS_Vector limitCoord;
    RS_Entity* trimEntity;
	RS_Vector trimCoord;
	bool both;
};

#endif
