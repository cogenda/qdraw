/****************************************************************************
** $Id: qg_graphicview.h 1563 2004-04-04 22:51:54Z andrew $
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

#ifndef QG_GRAPHICVIEW_H
#define QG_GRAPHICVIEW_H

#include <qwidget.h>
#include <qlabel.h>
#include <qscrollbar.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qurl.h>
#include <qfiledialog.h>

#include "rs_graphicview.h"
#include "rs_layerlistlistener.h"
#include "rs_blocklistlistener.h"
#include "rs_painterqt.h"

#include "qg_scrollbar.h"



/**
 * This is the Qt implementation of a widget which can view a 
 * graphic. 
 *
 * Instances of this class can be linked to layer lists using
 * addLayerListListener().
 */
class QG_GraphicView: public QWidget,
            public RS_GraphicView,
			public QFilePreview,
            public RS_LayerListListener,
    public RS_BlockListListener {
    Q_OBJECT

public:
    QG_GraphicView(QWidget* parent=0, const char* name=0, WFlags f=0);
    virtual ~QG_GraphicView();

    virtual int getWidth();
    virtual int getHeight();
    virtual void redraw();
    virtual void adjustOffsetControls();
    virtual void adjustZoomControls();
    virtual RS_Painter* createPainter();
    virtual RS_Painter* createDirectPainter();
    virtual void destroyPainter();
    virtual void setBackground(const RS_Color& bg);
    virtual void setMouseCursor(RS2::CursorType c);
	virtual void updateGridStatusWidget(const RS_String& text);


    // Methods from RS_LayerListListener Interface:
    virtual void layerEdited(RS_Layer*) {
        redraw();
    }
    virtual void layerRemoved(RS_Layer*) {
        redraw();
    }
    virtual void layerToggled(RS_Layer*) {
        redraw();
    }

protected:
    virtual void emulateMouseMoveEvent();
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
#if QT_VERSION>=0x030000	
	virtual void tabletEvent(QTabletEvent* e);
#endif	
    virtual void leaveEvent(QEvent*);
    virtual void enterEvent(QEvent*);
    virtual void focusInEvent(QFocusEvent*);
    virtual void focusOutEvent(QFocusEvent*);
    virtual void wheelEvent(QWheelEvent* e);
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);

    void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent* e);

	void previewUrl(const QUrl &u);

private slots:
    void slotHScrolled(int value);
    void slotVScrolled(int value);

private:
    //! Buffer for double-buffering
    QPixmap* buffer;

protected:
	int lastWidth;
	int lastHeight;
    //! Horizontal scrollbar.
    QG_ScrollBar* hScrollBar;
    //! Vertical scrollbar.
    QG_ScrollBar* vScrollBar;
    //! Layout used to fit in the view and the scrollbars.
    QGridLayout* layout;
	//! Label for grid spacing.
	QLabel* gridStatus;
    //! CAD mouse cursor
    QCursor* curCad;
    //! Delete mouse cursor
    QCursor* curDel;
    //! Select mouse cursor
    QCursor* curSelect;
    //! Magnifying glass mouse cursor
    QCursor* curMagnifier;
    //! Hand mouse cursor
    QCursor* curHand;
};

#endif
