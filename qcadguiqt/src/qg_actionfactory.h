/****************************************************************************
** $Id: qg_actionfactory.h 1346 2003-06-11 06:58:22Z andrew $
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

#ifndef QG_ACTIONFACTORY_H
#define QG_ACTIONFACTORY_H

#include <qaction.h>
#include <qobject.h>

#include "rs.h"

#include "qg_actionhandler.h"
#include "qg_mainwindowinterface.h"

/**
 * This class can store recent files in a list.
 */
class QG_ActionFactory : public QObject {
    Q_OBJECT

public:
    QG_ActionFactory(QG_ActionHandler* ah, QWidget* w);
    virtual ~QG_ActionFactory();

    QAction* createAction(RS2::ActionType id, QObject* obj);

private:
    QG_ActionHandler* actionHandler;
    QWidget* widget;
};

#endif

