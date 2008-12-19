/****************************************************************************
** $Id: qg_test.cpp 1346 2003-06-11 06:58:22Z andrew $
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

#include "qg_test.h"

#include "rs_creation.h"
#include "rs_import.h"
#include "rs_actiondrawpoint.h"

#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qkeycode.h>
#include <qtextedit.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qapplication.h>
#include <qaccel.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qwhatsthis.h>



QG_Test::QG_Test()
        : QMainWindow( 0, "example application main window", WDestructiveClose ) {
    resize(450, 600);

    graphic = new RS_Graphic();
    graphic->addEntity(new RS_Line(graphic,
                                   RS_LineData(RS_Vector(-10,-10), RS_Vector(100, 100))));

    graphicView = new QG_GraphicView(this, "graphicView");
    graphicView->setFocus();
    graphicView->setBorders(50,50,50,50);
    setCentralWidget(graphicView);

    RS_Import import(*graphic);
    import.fileImport("demo2.dxf");

    //eventHandler = new RS_EventHandler();
    action = new RS_ActionDrawPoint(*graphic, *graphicView);
    graphicView->setCurrentAction(action);

    graphicView->setContainer(graphic);
    //graphicView->setContainer(NULL);
    //graphicView->setEventHandler(eventHandler);

    statusBar()->message("Ready", 2000);
}


QG_Test::~QG_Test() {
    delete graphic;
    //delete eventHandler;
    delete graphicView;
}


void QG_Test::show() {
    QWidget::show();
    printf("w: %d / h: %d", graphicView->width(), graphicView->height());
    graphicView->zoomAuto();
    //graphicView->redraw();
}



void QG_Test::closeEvent(QCloseEvent* ce) {
    ce->accept();
}

