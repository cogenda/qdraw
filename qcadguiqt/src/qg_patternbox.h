/****************************************************************************
** $Id: qg_patternbox.h 1346 2003-06-11 06:58:22Z andrew $
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

#ifndef QG_PATTERNBOX_H
#define QG_PATTERNBOX_H

#include <qcombobox.h>

#include "rs.h"
#include "rs_pattern.h"
#include "rs_patternlist.h"

/**
 * A combobox for choosing a pattern name.
 */
class QG_PatternBox: public QComboBox {
    Q_OBJECT

public:
    QG_PatternBox(QWidget* parent=0, const char* name=0);
    virtual ~QG_PatternBox();

    RS_Pattern* getPattern() {
        return currentPattern;
    }
    void setPattern(const RS_String& pName);

    void init();

private slots:
    void slotPatternChanged(int index);

signals:
    void patternChanged(RS_Pattern* pattern);

private:
    RS_Pattern* currentPattern;
};

#endif

