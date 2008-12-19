/****************************************************************************
** $Id: qg_widthbox.h 1384 2003-08-25 22:51:59Z andrew $
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

#ifndef QG_WIDTHBOX_H
#define QG_WIDTHBOX_H

#include <qcombobox.h>

#include "rs.h"

/**
 * A combobox for choosing a line width.
 */
class QG_WidthBox: public QComboBox {
    Q_OBJECT

public:
    QG_WidthBox(QWidget* parent=0, const char* name=0);
    QG_WidthBox(bool showByLayer, bool showUnchanged,
                QWidget* parent=0, const char* name=0);
    virtual ~QG_WidthBox();

    RS2::LineWidth getWidth() {
        return currentWidth;
    }
    void setWidth(RS2::LineWidth w);
    void setLayerWidth(RS2::LineWidth w);

    void init(bool showByLayer, bool showUnchanged);

    bool isUnchanged() {
        return unchanged;
    }

private slots:
    void slotWidthChanged(int index);

signals:
    void widthChanged(RS2::LineWidth);

private:
    RS2::LineWidth currentWidth;
    bool showByLayer;
    bool showUnchanged;
    bool unchanged;
};

#endif

