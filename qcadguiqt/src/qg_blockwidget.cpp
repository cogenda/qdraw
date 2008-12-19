/****************************************************************************
** $Id: qg_blockwidget.cpp 1614 2004-12-09 23:10:15Z andrew $
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

#include "qg_blockwidget.h"

#include <qlayout.h>
#include <qtooltip.h>
#include <qpopupmenu.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qcursor.h>

#include "xpm/visibleblock.xpm"
#include "xpm/hiddenblock.xpm"
#include "xpm/blockadd.xpm"
#include "xpm/blockremove.xpm"
#include "xpm/blockedit.xpm"
#include "xpm/blockattributes.xpm"
#include "xpm/blockinsert.xpm"



/**
 * Constructor.
 */
QG_BlockWidget::QG_BlockWidget(QG_ActionHandler* ah, QWidget* parent,
                               const char* name, WFlags f)
        : QWidget(parent, name, f),
        pxmVisible(visibleblock_xpm),
        pxmHidden(hiddenblock_xpm),
        pxmAdd(blockadd_xpm),
        pxmRemove(blockremove_xpm),
        pxmAttributes(blockattributes_xpm),
        pxmEdit(blockedit_xpm),
        pxmInsert(blockinsert_xpm),
        pxmDefreezeAll(visibleblock_xpm),
pxmFreezeAll(hiddenblock_xpm) {

    actionHandler = ah;
    blockList = NULL;
	lastBlock = NULL;

    listBox = new QListBox(this, "blockbox");
    listBox->setDragSelect(false);
    listBox->setMultiSelection(false);
    listBox->setSmoothScrolling(true);
	listBox->setFocusPolicy(QWidget::NoFocus);

    QVBoxLayout* lay = new QVBoxLayout(this, 0, -1, "lay");

    /*
	QLabel* caption = new QLabel(tr("Block List"), this, "caption");
    caption->setAlignment(Qt::AlignCenter);
    caption->setPaletteBackgroundColor(black);
    caption->setPaletteForegroundColor(white);
	*/

    QHBoxLayout* layButtons = new QHBoxLayout(NULL, 0, -1, "layButtons");
    QHBoxLayout* layButtons2 = new QHBoxLayout(NULL, 0, -1, "layButtons2");
    QToolButton* but;
    // show all blocks:
    but = new QToolButton(this);
    but->setPixmap(pxmDefreezeAll);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Show all blocks"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotBlocksDefreezeAll()));
    layButtons->addWidget(but);
    // hide all blocks:
    but = new QToolButton(this);
    but->setPixmap(pxmFreezeAll);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Hide all blocks"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotBlocksFreezeAll()));
    layButtons->addWidget(but);
    // add block:
    but = new QToolButton(this);
    but->setPixmap(pxmAdd);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Add a block"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotBlocksAdd()));
    layButtons->addWidget(but);
    // remove block:
    but = new QToolButton(this);
    but->setPixmap(pxmRemove);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Remove the active block"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotBlocksRemove()));
    layButtons->addWidget(but);
    // edit attributes:
    but = new QToolButton(this);
    but->setPixmap(pxmAttributes);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Rename the active block"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotBlocksAttributes()));
    layButtons->addWidget(but);
    // edit block:
    but = new QToolButton(this);
    but->setPixmap(pxmEdit);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Edit the active block\n"
                          "in a separate window"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotBlocksEdit()));
    layButtons2->addWidget(but);
    // insert block:
    but = new QToolButton(this);
    but->setPixmap(pxmInsert);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Insert the active block"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotBlocksInsert()));
    layButtons2->addWidget(but);

    //lay->addWidget(caption);
    lay->addLayout(layButtons);
    lay->addLayout(layButtons2);
    lay->addWidget(listBox);

    //connect(listBox, SIGNAL(doubleClicked(QListBoxItem*)),
    //        actionHandler, SLOT(slotBlocksToggleView()));

    connect(listBox, SIGNAL(highlighted(const QString&)),
            this, SLOT(slotActivated(const QString&)));
	
	connect(listBox, SIGNAL(mouseButtonClicked(int, QListBoxItem*, 
		const QPoint&)),
		this, SLOT(slotMouseButtonClicked(int, QListBoxItem*, const QPoint&)));

    //boxLayout()->addWidget(listBox);
}



/**
 * Destructor
 */
QG_BlockWidget::~QG_BlockWidget() {
    delete listBox;
}



/**
 * Updates the block box from the blocks in the graphic.
 */
void QG_BlockWidget::update() {
    RS_DEBUG->print("QG_BlockWidget::update()");

    int yPos = listBox->contentsY();
    
	RS_Block* activeBlock;
    if (blockList!=NULL) {
        activeBlock = blockList->getActive();
    } else {
        activeBlock = NULL;
    }

    listBox->clear();

    if (blockList==NULL) {
		RS_DEBUG->print("QG_BlockWidget::update(): blockList is NULL");
        return;
    }

    for (uint i=0; i<blockList->count(); ++i) {
        RS_Block* blk = blockList->at(i);
        if (!blk->isFrozen()) {
            listBox->insertItem(pxmVisible, blk->getName());
        } else {
            listBox->insertItem(pxmHidden, blk->getName());
        }
    }

    listBox->sort();
	
	RS_Block* b = lastBlock;
    highlightBlock(activeBlock);
	lastBlock = b;
    listBox->setContentsPos(0, yPos);

    //highlightBlock(blockList->getActiveBlock());
    //listBox->setContentsPos(0, yPos);
    RS_DEBUG->print("QG_BlockWidget::update() done");
}



/**
 * Highlights (activates) the given block and makes it 
 * the active block in the blocklist.
 */
void QG_BlockWidget::highlightBlock(RS_Block* block) {
    RS_DEBUG->print("QG_BlockWidget::highlightBlock()");

    if (block==NULL || listBox==NULL) {
        return;
    }

    blockList->activate(block);
    QString name = block->getName();

    for (int i=0; i<(int)listBox->count(); ++i) {
        if (listBox->text(i)==name) {
            listBox->setCurrentItem(i);
            break;
        }
    }
}



/**
 * Toggles the view of the given block. This is usually called when 
 * an item is double clicked.
 */
/*
void QG_BlockWidget::slotToggleView(QListBoxItem* item) {
    RS_DEBUG->print("QG_BlockWidget::slotToggleView()");
 
    if (item==NULL || blockList==NULL) {
        return;
    }
 
    int index = listBox->index(item);
    RS_Block* block = blockList->find(item->text());
 
    if (block!=NULL) {
        blockList->toggleBlock(item->text());
        if (!block->isFrozen()) {
        	listBox->changeItem(pxmVisible, item->text(), index);
        } else {
            listBox->changeItem(*pxmHidden, item->text(), index);
        }
 
    }
}
*/



/**
 * Called when the user activates (highlights) a block.
 */
void QG_BlockWidget::slotActivated(const QString& blockName) {
    RS_DEBUG->print("QG_BlockWidget::slotActivated(): %s", blockName.latin1());

    if (blockList==NULL) {
        return;
    }

	lastBlock = blockList->getActive();

    blockList->activate(blockName);
}



/**
 * Called for every mouse click.
 */
void QG_BlockWidget::slotMouseButtonClicked(int /*button*/, 
    QListBoxItem* item, const QPoint& pos) {

	QPoint p = mapFromGlobal(pos);

	RS_Block* b = lastBlock;

	if (p.x()<23) {
		actionHandler->slotBlocksToggleView();
		highlightBlock(b);
	}
	else {
		if (item!=NULL && blockList!=NULL) {
			lastBlock = blockList->find(item->text());
		}
	}
}


/**
 * Shows a context menu for the block widget. Launched with a right click.
 */
#if QT_VERSION>=0x030000
void QG_BlockWidget::contextMenuEvent(QContextMenuEvent *e) {

    //QListBoxItem* item = listBox->selectedItem();
    QPopupMenu* contextMenu = new QPopupMenu(this);
    QLabel* caption = new QLabel(tr("Block Menu"), this);
    caption->setPaletteBackgroundColor(black);
    caption->setPaletteForegroundColor(white);
    caption->setAlignment( Qt::AlignCenter );
    contextMenu->insertItem( caption );
    contextMenu->insertItem( tr("&Defreeze all Blocks"), actionHandler,
                             SLOT(slotBlocksDefreezeAll()), 0);
    contextMenu->insertItem( tr("&Freeze all Blocks"), actionHandler,
                             SLOT(slotBlocksFreezeAll()), 0);
    contextMenu->insertItem( tr("&Add Block"), actionHandler,
                             SLOT(slotBlocksAdd()), 0);
    contextMenu->insertItem( tr("&Remove Block"), actionHandler,
                             SLOT(slotBlocksRemove()), 0);
    contextMenu->insertItem( tr("&Rename Block"), actionHandler,
                             SLOT(slotBlocksAttributes()), 0);
    contextMenu->insertItem( tr("&Edit Block"), actionHandler,
                             SLOT(slotBlocksEdit()), 0);
    contextMenu->insertItem( tr("&Insert Block"), actionHandler,
                             SLOT(slotBlocksInsert()), 0);
    contextMenu->insertItem( tr("&Toggle Visibility"), actionHandler,
                             SLOT(slotBlocksToggleView()), 0);
    contextMenu->insertItem( tr("&Create New Block"), actionHandler,
                             SLOT(slotBlocksCreate()), 0);
    contextMenu->exec(QCursor::pos());
    delete contextMenu;

    e->accept();
}
#endif



/**
 * Escape releases focus.
 */
void QG_BlockWidget::keyPressEvent(QKeyEvent* e) {
    switch (e->key()) {
    case Qt::Key_Escape:
        emit escape();
        break;

    default:
        QWidget::keyPressEvent(e);
        break;
    }
}

