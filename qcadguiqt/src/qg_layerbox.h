/****************************************************************************
** $Id: qg_layerbox.h 1383 2003-08-22 23:23:38Z andrew $
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

#ifndef QG_LAYERBOX_H
#define QG_LAYERBOX_H

#include <qcombobox.h>

#include "rs_layerlist.h"

/**
 * A combobox for choosing a layer.
 */
class QG_LayerBox: public QComboBox {
    Q_OBJECT

public:
    QG_LayerBox(QWidget* parent=0, const char* name=0);
    virtual ~QG_LayerBox();

    RS_Layer* getLayer() {
        return currentLayer;
    }
    void setLayer(RS_Layer& layer);
    void setLayer(RS_String& layer);

    void init(RS_LayerList& layerList, bool showByBlock, bool showUnchanged);

	bool isUnchanged() {
		return unchanged;
	}

private slots:
    void slotLayerChanged(int index);

signals:
    void layerChanged(RS_Layer* layer);

private:
    RS_LayerList* layerList;
    RS_Layer* currentLayer;
    bool showByBlock;
    bool showUnchanged;
	bool unchanged;
};

#endif

