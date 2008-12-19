/****************************************************************************
** $Id: qg_listviewitem.cpp 1385 2003-08-27 23:19:42Z andrew $
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

#include "qg_listviewitem.h"

#include <qpixmap.h>

#include "xpm/folderclosed.xpm"
#include "xpm/folderopen.xpm"

/**
 * Constructor for list view items with a folder icon.
 */
QG_ListViewItem::QG_ListViewItem(QG_ListViewItem* par,
                                 const QString& label,
                                 bool open,
                                 int id)
        : QListViewItem(par) {
    this->par = par;
    this->label = label;
    this->id = id;

    setPixmap(0, QPixmap((open ? folderopen_xpm : folderclosed_xpm)));
    setOpen(open);
}



/**
 * Constructor for root items.
 */
QG_ListViewItem::QG_ListViewItem(QListView * par,
                                 const QString& label,
                                 bool open,
                                 int id)
        : QListViewItem(par) {
		
    par = 0;
    this->label = label;
    this->id = id;

    setPixmap(0, QPixmap((open ? folderopen_xpm : folderclosed_xpm)));

    setOpen(open);
}



/**
 * Opens or closes the item.
 */
void QG_ListViewItem::setOpen(bool open) {
    if (open==true) {
        setPixmap(0, QPixmap(folderopen_xpm));
	}
    else {
        setPixmap(0, QPixmap(folderclosed_xpm));
	}

    QListViewItem::setOpen(open);
}



/**
 * Called in the beginning.
 */
void QG_ListViewItem::setup() {
    QListViewItem::setup();
}



/**
 * Returns the "path" of this item (like: "Project/Page1/Paragraph1/").
 */
QString QG_ListViewItem::getFullPath() {
    QString s;
    if (par!=NULL) {
        s = par->getFullPath();
        s.append(text(0));
        s.append("/");
    } else {
        s = text(0);
        s.append("/");
    }
    return s;
}


/**
 * Returns the text of the given column of this item.
 */
QString QG_ListViewItem::text(int column) const {
    if (column==0) {
        return label;
	}
    else {
        return "Column1";
	}
}

