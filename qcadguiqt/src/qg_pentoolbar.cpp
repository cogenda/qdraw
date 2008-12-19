/****************************************************************************
** $Id: qg_pentoolbar.cpp 1528 2004-03-07 00:07:37Z andrew $
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

#include "qg_pentoolbar.h"


/**
 * Constructor.
 */
QG_PenToolBar::QG_PenToolBar(QMainWindow* parent, const char* name)
        : QToolBar(parent, name) {

    colorBox = new QG_ColorBox(true, false, this, "colorbox");
    colorBox->setMinimumWidth(80);
    connect(colorBox, SIGNAL(colorChanged(const RS_Color&)),
            this, SLOT(slotColorChanged(const RS_Color&)));

    widthBox = new QG_WidthBox(true, false, this, "widthbox");
    widthBox->setMinimumWidth(80);
    connect(widthBox, SIGNAL(widthChanged(RS2::LineWidth)),
            this, SLOT(slotWidthChanged(RS2::LineWidth)));

    lineTypeBox = new QG_LineTypeBox(true, false, this, "lineTypebox");
    lineTypeBox->setMinimumWidth(80);
    connect(lineTypeBox, SIGNAL(lineTypeChanged(RS2::LineType)),
            this, SLOT(slotLineTypeChanged(RS2::LineType)));

    currentPen.setColor(colorBox->getColor());
    currentPen.setWidth(widthBox->getWidth());
    currentPen.setLineType(lineTypeBox->getLineType());
}


/**
 * Destructor
 */
QG_PenToolBar::~QG_PenToolBar() {}


/**
 * Called by the layer list if this object was added as a listener 
 * to a layer list.
 */
void QG_PenToolBar::layerActivated(RS_Layer* l) {

    //printf("QG_PenToolBar::layerActivated\n");

    if (l==NULL) {
        return;
    }

    //colorBox->setColor(l->getPen().getColor());
    //widthBox->setWidth(l->getPen().getWidth());

    colorBox->setLayerColor(l->getPen().getColor());
    widthBox->setLayerWidth(l->getPen().getWidth());
    lineTypeBox->setLayerLineType(l->getPen().getLineType());

    //if (colorBox->getColor().getFlag(C_BY_LAYER)) {
    //printf("  Color by layer\n");
    //colorBox->setColor(l->getPen().getColor());
    //}
}


/**
 * Called by the layer list (if this object was previously 
 * added as a listener to a layer list).
 */
void QG_PenToolBar::layerEdited(RS_Layer*) {}


/**
 * Called when the color was changed by the user.
 */
void QG_PenToolBar::slotColorChanged(const RS_Color& color) {
    currentPen.setColor(color);
    //printf("  color changed\n");

    emit penChanged(currentPen);
}

/**
 * Called when the width was changed by the user.
 */
void QG_PenToolBar::slotWidthChanged(RS2::LineWidth w) {
    currentPen.setWidth(w);
    //printf("  width changed\n");

    emit penChanged(currentPen);
}

/**
 * Called when the linetype was changed by the user.
 */
void QG_PenToolBar::slotLineTypeChanged(RS2::LineType w) {
    currentPen.setLineType(w);
    //printf("  line type changed\n");

    emit penChanged(currentPen);
}

