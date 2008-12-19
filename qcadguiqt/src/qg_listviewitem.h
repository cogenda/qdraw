/****************************************************************************
** $Id: qg_listviewitem.h 1385 2003-08-27 23:19:42Z andrew $
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

#ifndef QG_LISTVIEWITEM_H
#define QG_LISTVIEWITEM_H

#include <qlistview.h>


/**
 * An item in a hierarchical list view with a nice folder icon.
 */
class QG_ListViewItem : public QListViewItem {
public:
    QG_ListViewItem(QListView *par, const QString& label,
                    bool open=false, int id=-1);
    QG_ListViewItem(QG_ListViewItem *par, const QString& label,
                    bool open=false, int id=-1);

    QString getFullPath();
    QString text(int column) const;

    QString getLabel() const {
        return label;
    }

    void setOpen(bool o);
    void setup();

    void setId(int id) {
        this->id = id;
    }
    int getId() {
        return id;
    }

private:
    QG_ListViewItem* par;
    QString label;
    QString object;
    int id;
};

#endif

