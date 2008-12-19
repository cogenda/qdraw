/****************************************************************************
** $Id: qg_recentfiles.cpp 1478 2003-11-12 21:23:55Z andrew $
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

#include "qg_recentfiles.h"

#include "rs_debug.h"

/**
 * Constructor
 * @param number Number of files that can be stored in the list at maximum
 */
QG_RecentFiles::QG_RecentFiles(int number) {
    this->number = number;
}


/**
 * Destructor
 */
QG_RecentFiles::~QG_RecentFiles() {}

/**
 * Adds a file to the list of recently loaded files if 
 * it's not already in the list.
 */
void QG_RecentFiles::add(const QString& filename) {
	RS_DEBUG->print("QG_RecentFiles::add");
	
    // is the file already in the list?
    if (files.find(filename) != files.end()) {
        return;
    }

    // append
    //files.push_back(filename);
    files.append(filename);
    if ((int)files.count() > number) {
        // keep the list short
#if QT_VERSION>=0x030000
        files.pop_front();
#else
		// Crashes sometimes under XP !!:
		files.remove(files.first());
#endif
    }

    //for (int i=0; i<(int)files.count(); ++i) {
    //	printf("recent file[%d]: %s\n", i, files[i].latin1());
    //}
	RS_DEBUG->print("QG_RecentFiles::add: OK");
}


