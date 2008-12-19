/****************************************************************************
** $Id: qg_colorbox.cpp 1526 2004-03-05 23:01:24Z andrew $
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

#include "qg_colorbox.h"

#include <qcolordialog.h>
#include <qpainter.h>
#include <qpixmap.h>

#include "xpm/color00.xpm"
#include "xpm/color01.xpm"
#include "xpm/color02.xpm"
#include "xpm/color03.xpm"
#include "xpm/color04.xpm"
#include "xpm/color05.xpm"
#include "xpm/color06.xpm"
#include "xpm/color07.xpm"
#include "xpm/color08.xpm"
#include "xpm/color09.xpm"
#include "xpm/color11.xpm"
#include "xpm/color14.xpm"
#include "xpm/colorxx.xpm"


/**
 * Default Constructor. You must call init manually if you choose
 * to use this constructor.
 */
QG_ColorBox::QG_ColorBox(QWidget* parent, const char* name)
        : QComboBox(false, parent, name) {

    showByLayer = false;
    showUnchanged = false;
	unchanged = false;
}

/**
 * Constructor that calls init and provides a fully functional 
 * combobox for choosing colors.
 *
 * @param showByLayer true: Show attribute ByLayer, ByBlock.
 */
QG_ColorBox::QG_ColorBox(bool showByLayer, bool showUnchanged, 
	QWidget* parent, const char* name)
        : QComboBox(parent, name) {
	
	unchanged = false;
    init(showByLayer, showUnchanged);
}


/**
 * Destructor
 */
QG_ColorBox::~QG_ColorBox() {}


/**
 * Initialisation (called from constructor or manually but only
 * once).
 *
 * @param showByLayer true: Show attribute ByLayer, ByBlock.
 */
void QG_ColorBox::init(bool showByLayer, bool showUnchanged) {
    this->showByLayer = showByLayer;
    this->showUnchanged = showUnchanged;

    if (showUnchanged) {
		insertItem(QPixmap(color00_xpm), tr("Unchanged"));
	}
    if (showByLayer) {
        insertItem(QPixmap(color00_xpm), tr("By Layer"));
        insertItem(QPixmap(color00_xpm), tr("By Block"));
    }
    insertItem(QPixmap(color01_xpm), tr("Red"));
    insertItem(QPixmap(color02_xpm), tr("Yellow"));
    insertItem(QPixmap(color03_xpm), tr("Green"));
    insertItem(QPixmap(color04_xpm), tr("Cyan"));
    insertItem(QPixmap(color05_xpm), tr("Blue"));
    insertItem(QPixmap(color06_xpm), tr("Magenta"));
    insertItem(QPixmap(color07_xpm), tr("Black / White"));
    insertItem(QPixmap(color08_xpm), tr("Gray"));
    insertItem(QPixmap(color09_xpm), tr("Light Gray"));
    //insertItem(QPixmap(color11_xpm), tr("11"));
    //insertItem(QPixmap(color14_xpm), tr("14"));
    insertItem(QPixmap(colorxx_xpm), tr("Others.."));

    connect(this, SIGNAL(activated(int)),
            this, SLOT(slotColorChanged(int)));

	if (showUnchanged) {
        setCurrentItem(0);
	}
    else if (showByLayer) {
        setCurrentItem(0);
    } else {
        setCurrentItem(6);
    }

    slotColorChanged(currentItem());
}

/**
 * Sets the color shown in the combobox to the given color.
 */
void QG_ColorBox::setColor(const RS_Color& color) {
    currentColor = color;
	
    if (color.isByLayer() && showByLayer) {
        setCurrentItem(0);
    } else if (color.isByBlock() && showByLayer) {
        setCurrentItem(1);
    } else if (color==Qt::red) {
        setCurrentItem(0+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==Qt::yellow) {
        setCurrentItem(1+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==Qt::green) {
        setCurrentItem(2+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==Qt::cyan) {
        setCurrentItem(3+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==Qt::blue) {
        setCurrentItem(4+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==Qt::magenta) {
        setCurrentItem(5+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==Qt::white || color==Qt::black) {
        setCurrentItem(6+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==Qt::gray) {
        setCurrentItem(7+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==QColor(191,191,191)) {
        setCurrentItem(8+(int)showByLayer*2 + (int)showUnchanged);
    } else {
        setCurrentItem(9+(int)showByLayer*2 + (int)showUnchanged);
    }

    if (currentItem()!=9+(int)showByLayer*2 + (int)showUnchanged) {
        slotColorChanged(currentItem());
    }
}



/**
 * Sets the color of the pixmap next to the "By Layer" item
 * to the given color.
 */
void QG_ColorBox::setLayerColor(const RS_Color& color) {
    if (showByLayer) {
        QPixmap pixmap;
        if (color==Qt::black || color==Qt::white) {
            pixmap = color07_xpm;
        } else {
            pixmap = color00_xpm;
            int w = pixmap.width();
            int h = pixmap.height();
            QPainter painter(&pixmap);
            painter.fillRect(1, 1, w-2, h-2, color);
            painter.end();
        }

        changeItem(pixmap, tr("By Layer"), 0);

        // needed for the first time a layer is added:
        if (currentItem()!=9) {
            slotColorChanged(currentItem());
        }
    }
}



/**
 * Called when the color has changed. This method 
 * sets the current color to the value chosen or even
 * offers a dialog to the user that allows him/ her to
 * choose an individual color.
 */
void QG_ColorBox::slotColorChanged(int index) {
    currentColor.resetFlags();
	
    if (showUnchanged) {
		if (index==0) {
			unchanged = true;
		}
		else {
			unchanged = false;
		}
	}

    if (showByLayer) {
        switch (index-(int)showUnchanged) {
        case 0:
            currentColor = RS_Color(RS2::FlagByLayer);
            break;
        case 1:
            currentColor = RS_Color(RS2::FlagByBlock);
            break;
        default:
            break;
        }
    }

    switch (index-(int)showByLayer*2-(int)showUnchanged) {
    case 0:
        currentColor.setRgb(255, 0, 0);
        break;
    case 1:
        currentColor.setRgb(255, 255, 0);
        break;
    case 2:
        currentColor.setRgb(0, 255, 0);
        break;
    case 3:
        currentColor.setRgb(0, 255, 255);
        break;
    case 4:
        currentColor.setRgb(0, 0, 255);
        break;
    case 5:
        currentColor.setRgb(255, 0, 255);
        break;
    case 6:
        currentColor.setRgb(0, 0, 0);
        break;
    case 7:
        currentColor.setRgb(127, 127, 127);
        break;
    case 8:
        currentColor.setRgb(191, 191, 191);
        break;
    case 9:
#if QT_VERSION>=0x030000
        currentColor = QColorDialog::getColor(currentColor, this);
#endif
        break;
    default:
        break;
    }

    //printf("Current color is (%d): %s\n",
    //       index, currentColor.name().latin1());

    emit colorChanged(currentColor);
}


