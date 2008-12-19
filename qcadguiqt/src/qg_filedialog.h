/****************************************************************************
** $Id: qg_filedialog.h 1505 2004-01-25 01:06:24Z andrew $
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

#ifndef QG_FILEDIALOG_H
#define QG_FILEDIALOG_H

#include "rs.h"

/**
 * File Open / Save dialogs.
 */
class QG_FileDialog {
public:
    //QG_FileDialog(QWidget* parent=0, const char* name=0, WFlags f=0);
    //virtual ~QG_FileDialog();

    static QString getOpenFileName(QWidget* parent, RS2::FormatType* type=NULL);
    static QString getSaveFileName(QWidget* parent, RS2::FormatType* type=NULL);

	
};

#endif
