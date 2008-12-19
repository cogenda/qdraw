/****************************************************************************
** $Id: rs_python.h 1960 2005-03-12 12:22:01Z andrew $
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

#ifndef RS_PYTHON_H
#define RS_PYTHON_H

#ifdef RS_OPT_PYTHON

#include "Python.h"

#include "rs_graphic.h"

#define RS_PYTHON RS_Python::instance()

/**
 * Python scripting support.
 *
 * OBSOLETE
 *
 * @author Andrew Mustun
 */
class RS_Python {
private:
    RS_Python();

public:
    static RS_Python* instance();

    void setGraphic(RS_Graphic* g) {
        graphic = g;
    }

    RS_Graphic* getGraphic() {
        return graphic;
    }

    int launch(const RS_String& script);

private:
    static RS_Python* uniqueInstance;
    RS_Graphic* graphic;
};

#endif

#endif
