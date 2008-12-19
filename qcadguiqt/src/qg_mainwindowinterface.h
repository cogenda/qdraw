/****************************************************************************
** $Id: qg_mainwindowinterface.h 1617 2004-12-17 04:32:05Z andrew $
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

#ifndef QG_MAINWINDOWINTERFACE_H
#define QG_MAINWINDOWINTERFACE_H

#include <qmainwindow.h>

#include "rs_document.h"
#include "rs_mainwindowinterface.h"

#include "qg_graphicview.h"
#include "qg_actionhandler.h"



/**
 * Interface for main application windows.
 */
class QG_MainWindowInterface : public RS_MainWindowInterface {

public:
    QG_MainWindowInterface() {}
    virtual ~QG_MainWindowInterface() {}

    virtual QMainWindow* getMainWindow() = 0;
    virtual QG_ActionHandler* getActionHandler() = 0;
	virtual void setFocus2() = 0;

	//virtual QToolBar* createToolBar(const RS_String& name) = 0;
	//virtual void addToolBarButton(QToolBar* tb) = 0;
};

#endif

