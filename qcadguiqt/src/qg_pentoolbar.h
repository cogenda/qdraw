/****************************************************************************
** $Id: qg_pentoolbar.h 1346 2003-06-11 06:58:22Z andrew $
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

#ifndef QG_PENTOOLBAR_H
#define QG_PENTOOLBAR_H

#include <qtoolbar.h>

#include "rs_layerlistlistener.h"
#include "rs_pen.h"

#include "qg_colorbox.h"
#include "qg_widthbox.h"
#include "qg_linetypebox.h"

/**
 * A toolbar that offers all widgets for choosing a pen.
 */
class QG_PenToolBar: public QToolBar,
    public RS_LayerListListener {
    Q_OBJECT

public:
    QG_PenToolBar(QMainWindow* parent=0, const char* name=0);
    virtual ~QG_PenToolBar();

    RS_Pen getPen() {
        return currentPen;
    }

    // Methods from RS_LayerListListener Interface:
    virtual void layerActivated(RS_Layer*);
    virtual void layerEdited(RS_Layer*);

public slots:
    void slotColorChanged(const RS_Color& color);
    void slotWidthChanged(RS2::LineWidth w);
    void slotLineTypeChanged(RS2::LineType w);

signals:
    void penChanged(RS_Pen);

private:
    RS_Pen currentPen;
    QG_ColorBox* colorBox;
    QG_WidthBox* widthBox;
    QG_LineTypeBox* lineTypeBox;
};

#endif

