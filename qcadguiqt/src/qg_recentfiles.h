/****************************************************************************
** $Id: qg_recentfiles.h 1346 2003-06-11 06:58:22Z andrew $
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

#ifndef QG_RECENTFILES_H
#define QG_RECENTFILES_H

#include <qstring.h>
#include <qstringlist.h>

/**
 * This class can store recent files in a list.
 */
class QG_RecentFiles {

public:
    QG_RecentFiles(int number);
    virtual ~QG_RecentFiles();

    void add(const QString& filename);

    /**
     * @return complete path and name of the file stored in the 
     * list at index i 
     */
    QString get(int i) {
        if (i<(int)files.count()) {
            return files[i];
        } else {
            return QString("");
        }
    }

    /** @return number of files currently stored in the list */
    int count() {
        return files.count();
    }

    /** @return number of files that can be stored in the list at maximum */
    int getNumber() {
        return number;
    }

private:
    int number;
    QStringList files;
};

#endif

