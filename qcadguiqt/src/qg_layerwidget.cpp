/****************************************************************************
** $Id: qg_layerwidget.cpp 1614 2004-12-09 23:10:15Z andrew $
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

#include "qg_layerwidget.h"

#include <qtoolbutton.h>
#include <qcursor.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpopupmenu.h>
#include <qtooltip.h>

#include "xpm/layerstatus_00.xpm"
#include "xpm/layerstatus_01.xpm"
#include "xpm/layerstatus_10.xpm"
#include "xpm/layerstatus_11.xpm"
#include "xpm/visibleblock.xpm"
#include "xpm/hiddenblock.xpm"
#include "xpm/layeradd.xpm"
#include "xpm/layerremove.xpm"
#include "xpm/layeredit.xpm"



/**
 * Constructor.
 */
QG_LayerWidget::QG_LayerWidget(QG_ActionHandler* ah, QWidget* parent,
                               const char* name, WFlags f)
        : QWidget(parent, name, f),
        pxmLayerStatus00(layerstatus_00_xpm),
        pxmLayerStatus01(layerstatus_01_xpm),
        pxmLayerStatus10(layerstatus_10_xpm),
        pxmLayerStatus11(layerstatus_11_xpm),
        pxmVisible(visibleblock_xpm),
        pxmHidden(hiddenblock_xpm),
        pxmAdd(layeradd_xpm),
        pxmRemove(layerremove_xpm),
        pxmEdit(layeredit_xpm),
        pxmDefreezeAll(visibleblock_xpm),
        pxmFreezeAll(hiddenblock_xpm) {

    actionHandler = ah;
    layerList = NULL;
    showByBlock = false;
	lastLayer = NULL;

    listBox = new QListBox(this, "layerbox");
    listBox->setDragSelect(false);
    listBox->setMultiSelection(false);
    listBox->setSmoothScrolling(true);
    listBox->setFocusPolicy(QWidget::NoFocus);
    listBox->setMinimumHeight(140);

    QVBoxLayout* lay = new QVBoxLayout(this, 0, -1, "lay");

    /*QLabel* caption = new QLabel(tr("Layer List"), this, "lLayers");
    caption->setAlignment(Qt::AlignCenter);
    caption->setPaletteBackgroundColor(black);
    caption->setPaletteForegroundColor(white);
    */

    QHBoxLayout* layButtons = new QHBoxLayout(NULL, 0, -1, "layButtons");
    QToolButton* but;
    // show all layer:
    but = new QToolButton(this);
    but->setPixmap(pxmDefreezeAll);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Show all layers"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotLayersDefreezeAll()));
    layButtons->addWidget(but);
    // hide all layer:
    but = new QToolButton(this);
    but->setPixmap(pxmFreezeAll);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Hide all layers"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotLayersFreezeAll()));
    layButtons->addWidget(but);
    // add layer:
    but = new QToolButton(this);
    but->setPixmap(pxmAdd);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Add a layer"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotLayersAdd()));
    layButtons->addWidget(but);
    // remove layer:
    but = new QToolButton(this);
    but->setPixmap(pxmRemove);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Remove the current layer"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotLayersRemove()));
    layButtons->addWidget(but);
    // rename layer:
    but = new QToolButton(this);
    but->setPixmap(pxmEdit);
    but->setMinimumSize(QSize(22,22));
    QToolTip::add(but, tr("Modify layer attributes / rename"));
    connect(but, SIGNAL(clicked()),
            actionHandler, SLOT(slotLayersEdit()));
    layButtons->addWidget(but);

    //lay->addWidget(caption);
    lay->addLayout(layButtons);
    lay->addWidget(listBox);

    connect(listBox, SIGNAL(highlighted(const QString&)),
            this, SLOT(slotActivated(const QString&)));

    //connect(listBox, SIGNAL(doubleClicked(QListBoxItem*)),
    //        actionHandler, SLOT(slotLayersToggleView()));

    connect(listBox, SIGNAL(mouseButtonClicked(int, QListBoxItem*, 
        const QPoint&)),
        this, SLOT(slotMouseButtonClicked(int, QListBoxItem*, const QPoint&)));
}



/**
 * Destructor
 */
QG_LayerWidget::~QG_LayerWidget() {
    delete listBox;
    //delete pxmVisible;
    //delete pxmHidden;
}



/**
 * Sets the layerlist this layer widget should show.
 *
 * @param showByBlock true: show the layer with the name "ByBlock" if
 *                    it exists.
 *                    false: don't show special layer "ByBlock"
 */
void QG_LayerWidget::setLayerList(RS_LayerList* layerList, bool showByBlock) {
    this->layerList = layerList;
    this->showByBlock = showByBlock;
    update();
}



/**
 * Updates the layer box from the layers in the graphic.
 */
void QG_LayerWidget::update() {
    RS_DEBUG->print("QG_LayerWidget::update() begin");

    int yPos = listBox->contentsY();

    RS_Layer* activeLayer;
    if (layerList!=NULL) {
        activeLayer = layerList->getActive();
    } else {
        activeLayer = NULL;
    }
	
    RS_DEBUG->print("QG_LayerWidget::update() clearing listBox");

    listBox->clear();

    if (layerList==NULL) {
        RS_DEBUG->print("QG_LayerWidget::update() abort");
        return;
    }
	
    RS_DEBUG->print("QG_LayerWidget::update() filling in layers");

    for (uint i=0; i<layerList->count(); ++i) {
        RS_Layer* layer = layerList->at(i);

        // hide layer "ByBlock"?
        if (showByBlock || layer->getName()!="ByBlock") {
            QPixmap* pm = NULL;
            
            if (!layer->isFrozen()) {
                if (!layer->isLocked()) {
                    pm = &pxmLayerStatus10;
                }
                else {
                    pm = &pxmLayerStatus11;
                }
            } else {
                if (!layer->isLocked()) {
                    pm = &pxmLayerStatus00;
                }
                else {
                    pm = &pxmLayerStatus01;
                }
            }

            if (pm!=NULL) {
                listBox->insertItem(*pm, layer->getName());
            }
        }
    }
	
    RS_DEBUG->print("QG_LayerWidget::update() sorting");

    listBox->sort();
	
    RS_DEBUG->print("QG_LayerWidget::update() reactivating current layer");

	RS_Layer* l = lastLayer;
    highlightLayer(activeLayer);
	lastLayer = l;
    listBox->setContentsPos(0, yPos);

    RS_DEBUG->print("QG_LayerWidget::update() end");
}


/**
 * Highlights (activates) the given layer and makes it 
 * the active layer in the layerlist.
 */
void QG_LayerWidget::highlightLayer(RS_Layer* layer) {
    RS_DEBUG->print("QG_LayerWidget::highlightLayer() begin");

    if (layer==NULL || layerList==NULL) {
        RS_DEBUG->print("QG_LayerWidget::highlightLayer() abort");
        return;
    }

    QString name = layer->getName();
	highlightLayer(name);

    RS_DEBUG->print("QG_LayerWidget::highlightLayer() end");
}



/**
 * Highlights (activates) the given layer and makes it 
 * the active layer in the layerlist.
 */
void QG_LayerWidget::highlightLayer(const QString& name) {
    RS_DEBUG->print("QG_LayerWidget::highlightLayer(name) begin");

    if (layerList==NULL) {
        RS_DEBUG->print("QG_LayerWidget::highlightLayer(name) abort");
        return;
    }

    layerList->activate(name);

    for (int i=0; i<(int)listBox->count(); ++i) {
        if (listBox->text(i)==name) {
            listBox->setCurrentItem(i);
            break;
        }
    }

    RS_DEBUG->print("QG_LayerWidget::highlightLayer(name) end");
}



/**
 * Called when the user activates (highlights) a layer.
 */
void QG_LayerWidget::slotActivated(const QString& layerName) {
    RS_DEBUG->print("QG_LayerWidget::slotActivated(): %s", layerName.latin1());

    if (layerList==NULL) {
        return;
    }

	lastLayer = layerList->getActive();
	
    layerList->activate(layerName);
}


/**
 * Called for every mouse click.
 */
void QG_LayerWidget::slotMouseButtonClicked(int /*button*/, 
   QListBoxItem* item, const QPoint& pos) {
   
    RS_DEBUG->print("QG_LayerWidget::slotMouseButtonClicked()");

    QPoint p = mapFromGlobal(pos);
	
	// only change state / no activation
	RS_Layer* l = lastLayer;
	
    if (p.x()<23) {
        actionHandler->slotLayersToggleView();
		highlightLayer(l);
    }
    else if (p.x()<34) {
        actionHandler->slotLayersToggleLock();
		highlightLayer(l);
    }
	else {
		if (item!=NULL && layerList!=NULL) {
			lastLayer = layerList->find(item->text());
		}
	}
}



/**
 * Shows a context menu for the layer widget. Launched with a right click.
 */
#if QT_VERSION>=0x030000
void QG_LayerWidget::contextMenuEvent(QContextMenuEvent *e) {

    if (actionHandler!=NULL) {
        QPopupMenu* contextMenu = new QPopupMenu(this);
        QLabel* caption = new QLabel(tr("Layer Menu"), this);
        caption->setPaletteBackgroundColor(black);
        caption->setPaletteForegroundColor(white);
        caption->setAlignment( Qt::AlignCenter );
        contextMenu->insertItem( caption );
        contextMenu->insertItem( tr("&Defreeze all Layers"), actionHandler,
                                 SLOT(slotLayersDefreezeAll()), 0);
        contextMenu->insertItem( tr("&Freeze all Layers"), actionHandler,
                                 SLOT(slotLayersFreezeAll()), 0);
        contextMenu->insertItem( tr("&Add Layer"), actionHandler,
                                 SLOT(slotLayersAdd()), 0);
        contextMenu->insertItem( tr("&Remove Layer"), actionHandler,
                                 SLOT(slotLayersRemove()), 0);
        contextMenu->insertItem( tr("&Edit Layer"), actionHandler,
                                 SLOT(slotLayersEdit()), 0);
        contextMenu->insertItem( tr("&Toggle Visibility"), actionHandler,
                                 SLOT(slotLayersToggleView()), 0);
        contextMenu->exec(QCursor::pos());
        delete contextMenu;
    }

    e->accept();
}
#endif


/**
 * Escape releases focus.
 */
void QG_LayerWidget::keyPressEvent(QKeyEvent* e) {
    switch (e->key()) {
    
    case Qt::Key_Escape:
        emit escape();
        break;
        
    default:
        QWidget::keyPressEvent(e);
        break;
    }
}

