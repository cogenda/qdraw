/****************************************************************************
** $Id: main.cpp 1346 2003-06-11 06:58:22Z andrew $
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

#include <qapplication.h>
#include "qg_test.h"

int main( int argc, char ** argv ) {
    QApplication a(argc, argv);
    QG_Test * mw = new QG_Test();

    mw->setCaption( "QCad GUI Test Application" );
    mw->show();

    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

    return a.exec();
}
