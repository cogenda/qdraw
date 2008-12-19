/****************************************************************************
** $Id: main.h 2386 2005-05-16 17:08:57Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the QCad project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid QCad Professional Edition licenses may use 
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

#ifndef MAIN_H
#define MAIN_H

#include <qstringlist.h>

#define QC_VERSION "2.0.5.0"
#ifndef QC_APPNAME
# ifdef RS_DEMO
#  define QC_APPNAME "QCad Demo"
# else
#  define QC_APPNAME "QCad"
# endif
#endif

#ifndef QC_DEBUGGING
# define QC_DEBUGGING false
#endif

#ifndef QC_COMPANYKEY
#define QC_COMPANYKEY "/RibbonSoft"
#endif

QStringList handleArgs(int argc, char** argv);

#endif



