/****************************************************************************
** $Id: qg_colorbox.h 1383 2003-08-22 23:23:38Z andrew $
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

#ifndef QG_COLORBOX_H
#define QG_COLORBOX_H

#include <qcombobox.h>

#include "rs_color.h"

/**
 * A combobox for choosing a color.
 */
class QG_ColorBox: public QComboBox {
    Q_OBJECT

public:
    QG_ColorBox(QWidget* parent=0, const char* name=0);
    QG_ColorBox(bool showByLayer, bool showUnchanged, 
		QWidget* parent=0, const char* name=0);
    virtual ~QG_ColorBox();

    RS_Color getColor() {
        return currentColor;
    }

    void setColor(const RS_Color& color);
    void setLayerColor(const RS_Color& color);

    void init(bool showByLayer, bool showUnchanged);

	bool isUnchanged() {
		return unchanged;
	}

private slots:
    void slotColorChanged(int index);

signals:
    void colorChanged(const RS_Color& color);

private:
    RS_Color currentColor;
    bool showByLayer;
	bool showUnchanged;
	bool unchanged;
};

#endif

