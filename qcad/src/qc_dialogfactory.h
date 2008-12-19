/****************************************************************************
** $Id: qc_dialogfactory.h 893 2004-07-29 20:33:02Z andrew $
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

#ifndef QC_DIALOGFACTORY_H
#define QC_DIALOGFACTORY_H

#include "qg_dialogfactory.h"


/**
 * This is the QCad specific implementation of a widget which can create and
 * show dialogs. Some functions cannot be implemented on the
 * qcadguiqt library level and need to be implemented here,
 * on the application level.
 */
class QC_DialogFactory: public QG_DialogFactory {
public:
    QC_DialogFactory(QWidget* parent, QWidget* ow) :
	  QG_DialogFactory(parent, ow) {}
    virtual ~QC_DialogFactory() {}

    virtual void requestEditBlockWindow(RS_BlockList* blockList = NULL);
    virtual void closeEditBlockWindow(RS_Block* block = NULL);
};

#endif
