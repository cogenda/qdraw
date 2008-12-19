/****************************************************************************
** $Id: qg_patternbox.cpp 1494 2003-12-29 23:05:37Z andrew $
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

#include "qg_patternbox.h"

#include <qpixmap.h>
#include <qstringlist.h>

#include "rs_debug.h"


/**
 * Default Constructor. You must call init manually if you choose
 * to use this constructor.
 */
QG_PatternBox::QG_PatternBox(QWidget* parent, const char* name)
        : QComboBox(parent, name) {
}



/**
 * Destructor
 */
QG_PatternBox::~QG_PatternBox() {}


/**
 * Initialisation (called manually and only once).
 */
void QG_PatternBox::init() {
    QStringList patterns;

    for (RS_Pattern* f = RS_PATTERNLIST->firstPattern();
            f!=NULL;
            f = RS_PATTERNLIST->nextPattern()) {

        patterns.append(f->getFileName());
    }

    patterns.sort();
    insertStringList(patterns);

    connect(this, SIGNAL(activated(int)),
            this, SLOT(slotPatternChanged(int)));

    setCurrentItem(0);
    slotPatternChanged(currentItem());
}



/**
 * Sets the currently selected width item to the given width.
 */
void QG_PatternBox::setPattern(const RS_String& pName) {

    RS_DEBUG->print("QG_PatternBox::setPattern %s\n", pName.latin1());

#if QT_VERSION>=0x030000
    setCurrentText(pName);
#else
    setEditText(pName);
#endif

    slotPatternChanged(currentItem());
}



/**
 * Called when the pattern has changed. This method 
 * sets the current pattern to the value chosen.
 */
void QG_PatternBox::slotPatternChanged(int index) {

    RS_DEBUG->print("QG_PatternBox::slotPatternChanged %d\n", index);

    currentPattern = RS_PATTERNLIST->requestPattern(currentText());

    if (currentPattern!=NULL) {
        RS_DEBUG->print("Current pattern is (%d): %s\n",
                        index, currentPattern->getFileName().latin1());
    }

    emit patternChanged(currentPattern);
}


