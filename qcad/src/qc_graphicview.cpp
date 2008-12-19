/****************************************************************************
** $Id: qc_graphicview.cpp 861 2004-04-04 22:52:13Z andrew $
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

#include "qc_graphicview.h"


#include "rs_actiondefault.h"
#include "rs_settings.h"

#include "qc_applicationwindow.h"

QC_GraphicView::QC_GraphicView(RS_Document* doc, QWidget* parent)
        :QG_GraphicView(parent, "graphicview") {

    RS_DEBUG->print("QC_GraphicView::QC_GraphicView()..");
    //document = doc;

    RS_DEBUG->print("  Setting Container..");
    if (doc!=NULL) {
        setContainer(doc);
    }
    RS_DEBUG->print("  container set.");
    setFactorX(4.0);
    setFactorY(4.0);
    setOffset(50, 50);
    setBorders(10, 10, 10, 10);
	
    //setBackground(Qt::black);

	if (doc!=NULL) {
		setDefaultAction(new RS_ActionDefault(*doc, *this));
	}
}


QC_GraphicView::~QC_GraphicView() {
}

void QC_GraphicView::drawIt() {
    if (!isUpdateEnabled()) {
        return;
    }

    //createPainter();

    RS_GraphicView::drawIt();

    //destroyPainter();
}


// EOF
