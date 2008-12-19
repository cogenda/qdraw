/****************************************************************************
** $Id: qg_qt2rs.h 1346 2003-06-11 06:58:22Z andrew $
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

#ifndef QG_QT2RS_H
#define QG_QT2RS_H

#include <qnamespace.h>

#include "rs_mouseevent.h"

/**
 * Translates Qt stuff into qcadlib stuff.
 */
class QG_Qt2Rs {

public:
    QG_Qt2Rs() {}
    ~QG_Qt2Rs() {}

    /*static RS_MouseEvent mouseEvent(QMouseEvent* e) {
    	RS_MouseButton button;
    	switch (e->button()) {
    		case Qt::LeftButton:
    			button = LEFT;
    			break;
    		case Qt::RightButton:
    			button = RIGHT;
    			break;
    		case Qt::MidButton:
    			button = MIDDLE;
    			break;
    		default:
    			button = NONE;
    			break;
    	}

    	return RS_MouseEvent(e->pos().x(), 
                                e->pos().y(), 
                                button );
}*/
}
;

#endif

