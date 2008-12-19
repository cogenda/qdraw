/****************************************************************************
** $Id: qg_blockwidget.h 1563 2004-04-04 22:51:54Z andrew $
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

#ifndef QG_BLOCKWIDGET_H
#define QG_BLOCKWIDGET_H

#include <qwidget.h>
#include <qlistbox.h>

#include "rs_blocklist.h"
#include "rs_blocklistlistener.h"

#include "qg_actionhandler.h"

/**
 * This is the Qt implementation of a widget which can view a 
 * block list.
 */
class QG_BlockWidget: public QWidget, public RS_BlockListListener {
    Q_OBJECT

public:
    QG_BlockWidget(QG_ActionHandler* ah, QWidget* parent,
                   const char* name=0, WFlags f = 0);
    ~QG_BlockWidget();

    void setBlockList(RS_BlockList* blockList) {
        this->blockList = blockList;
        update();
    }

    RS_BlockList* getBlockList() {
        return blockList;
    }

    void update();
    void highlightBlock(RS_Block* block);

    //virtual void blockActivated(RS_Block *) {}
    virtual void blockAdded(RS_Block*) {
        update();
    }
    virtual void blockEdited(RS_Block*) {
        update();
    }
    virtual void blockRemoved(RS_Block*) {
        update();
    }
    virtual void blockToggled(RS_Block*) {
		update();
	}

signals:
	void escape();

public slots:
    //void slotToggleView(QListBoxItem* item);
    void slotActivated(const QString& blockName);
	void slotMouseButtonClicked(int button, QListBoxItem* item, 
		const QPoint& pos);

protected:
#if QT_VERSION>=0x030000
    void contextMenuEvent(QContextMenuEvent *e);
#endif
	virtual void keyPressEvent(QKeyEvent* e);

private:
    RS_BlockList* blockList;
    QListBox* listBox;
	RS_Block* lastBlock;
    QPixmap pxmVisible;
    QPixmap pxmHidden;
    QPixmap pxmAdd;
    QPixmap pxmRemove;
    QPixmap pxmAttributes;
    QPixmap pxmEdit;
    QPixmap pxmInsert;
    QPixmap pxmDefreezeAll;
    QPixmap pxmFreezeAll;
    QG_ActionHandler* actionHandler;
};

#endif
