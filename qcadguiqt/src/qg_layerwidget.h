/****************************************************************************
** $Id: qg_layerwidget.h 1589 2004-07-14 21:28:54Z andrew $
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

#ifndef QG_LAYERWIDGET_H
#define QG_LAYERWIDGET_H

#include <qwidget.h>
#include <qlistbox.h>

#include "rs_layerlist.h"
#include "rs_layerlistlistener.h"

#include "qg_actionhandler.h"



/**
 * This is the Qt implementation of a widget which can view a 
 * layer list and provides a user interface for basic layer actions.
 */
class QG_LayerWidget: public QWidget, public RS_LayerListListener {
    Q_OBJECT

public:
    QG_LayerWidget(QG_ActionHandler* ah, QWidget* parent,
                   const char* name=0, WFlags f = 0);
    ~QG_LayerWidget();

    void setLayerList(RS_LayerList* layerList, bool showByBlock);

    void update();
    void highlightLayer(RS_Layer* layer);
    void highlightLayer(const QString& name);

    virtual void layerActivated(RS_Layer* layer) {
        highlightLayer(layer);
    }
    virtual void layerAdded(RS_Layer* layer) {
        update();
        highlightLayer(layer);
    }
    virtual void layerEdited(RS_Layer*) {
        update();
    }
    virtual void layerRemoved(RS_Layer*) {
        update();
        highlightLayer(layerList->at(0));
    }
    virtual void layerToggled(RS_Layer*) {
        update();
    }

signals:
	void escape();

public slots:
    void slotActivated(const QString& layerName);
	void slotMouseButtonClicked(int button, QListBoxItem* item, 
		const QPoint& pos);

protected:
#if QT_VERSION>=0x030000
    void contextMenuEvent(QContextMenuEvent *e);
#endif
	virtual void keyPressEvent(QKeyEvent* e);

private:
    RS_LayerList* layerList;
    bool showByBlock;
    QListBox* listBox;
	RS_Layer* lastLayer;
    QPixmap pxmLayerStatus00;
    QPixmap pxmLayerStatus01;
    QPixmap pxmLayerStatus10;
    QPixmap pxmLayerStatus11;
    QPixmap pxmVisible;
    QPixmap pxmHidden;
    QPixmap pxmAdd;
    QPixmap pxmRemove;
    QPixmap pxmEdit;
    QPixmap pxmDefreezeAll;
    QPixmap pxmFreezeAll;
    QG_ActionHandler* actionHandler;
};

#endif
