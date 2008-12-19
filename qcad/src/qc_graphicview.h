/****************************************************************************
** $Id: qc_graphicview.h 861 2004-04-04 22:52:13Z andrew $
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

#ifndef QC_GRAPHICVIEW_H
#define QC_GRAPHICVIEW_H

#include <qwidget.h>

#include "rs_document.h"
#include "rs_eventhandler.h"
//#include "rs_painterqt.h"

#include "qg_graphicview.h"

class QC_ApplicationWindow;

/**
 * A view widget for the visualisation of drawings.
 * Very thin wrapper for QCad specific settings.
 *
 * @author Andrew Mustun
 */
class QC_GraphicView : public QG_GraphicView {
    Q_OBJECT

public:
    QC_GraphicView(RS_Document* doc, QWidget* parent=0);
    virtual ~QC_GraphicView();

    virtual void drawIt();


private:
    //RS_Document* document;
};

#endif

