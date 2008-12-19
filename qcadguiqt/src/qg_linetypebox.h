/****************************************************************************
** $Id: qg_linetypebox.h 1383 2003-08-22 23:23:38Z andrew $
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

#ifndef QG_LINETYPEBOX_H
#define QG_LINETYPEBOX_H

#include <qcombobox.h>

#include "rs.h"

/**
 * A combobox for choosing a line type.
 */
class QG_LineTypeBox: public QComboBox {
    Q_OBJECT

public:
    QG_LineTypeBox(QWidget* parent=0, const char* name=0);
    QG_LineTypeBox(bool showByLayer, bool showUnchanged, 
		QWidget* parent=0, const char* name=0);
    virtual ~QG_LineTypeBox();

    RS2::LineType getLineType() {
        return currentLineType;
    }
    void setLineType(RS2::LineType w);
    void setLayerLineType(RS2::LineType w);

    void init(bool showByLayer, bool showUnchanged);
	
	bool isUnchanged() {
		return unchanged;
	}

private slots:
    void slotLineTypeChanged(int index);

signals:
    void lineTypeChanged(RS2::LineType);

private:
    RS2::LineType currentLineType;
    bool showByLayer;
	bool showUnchanged;
	bool unchanged;
};

#endif

