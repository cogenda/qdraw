/****************************************************************************
** $Id: qg_commandedit.h 1370 2003-08-02 00:02:00Z andrew $
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

#ifndef QG_COMMANDEDIT_H
#define QG_COMMANDEDIT_H

#include <qlineedit.h>
#include <qstringlist.h>

/**
 * A command line edit with some typical console features 
 * (uparrow for the history, tab, ..).
 */
class QG_CommandEdit: public QLineEdit {
    Q_OBJECT

public:
    QG_CommandEdit(QWidget* parent=0, const char* name=0);
    virtual ~QG_CommandEdit();

protected:
	virtual bool event(QEvent* e);
	virtual void keyPressEvent(QKeyEvent* e);
	virtual void focusInEvent(QFocusEvent *e);
	virtual void focusOutEvent(QFocusEvent *e);

signals:
	void tabPressed();
	void escape();
	void focusIn();
	void focusOut();

private:
	QStringList historyList;
	QStringList::Iterator it;
	bool acceptCoordinates;
};

#endif

