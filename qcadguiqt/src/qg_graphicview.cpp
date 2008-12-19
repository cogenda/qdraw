/****************************************************************************
** $Id: qg_graphicview.cpp 1614 2004-12-09 23:10:15Z andrew $
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

#include "qg_graphicview.h"

#include <qbitmap.h>
#include <qcursor.h>
#include <qlabel.h>

#include "rs_actionzoomin.h"
#include "rs_actionzoompan.h"
#include "rs_actionzoomscroll.h"
#include "rs_actionmodifydelete.h"
#include "rs_actionselectsingle.h"
#include "rs_fileinfo.h"
#include "rs_graphic.h"
#include "rs_settings.h"
#include "rs_system.h"
#include "rs_painterqt.h"
#include "rs_patternlist.h"

#ifndef QCAD_EMBEDDED
#include "qg_cadtoolbar.h"
#endif

#include "qg_dialogfactory.h"
#include "qg_qt2rs.h"

#ifndef __APPLE__
#include "xpm/cur_glass_bmp.xpm"
#include "xpm/cur_glass_mask.xpm"
#include "xpm/cur_cad_bmp.xpm"
#include "xpm/cur_cad_mask.xpm"
#include "xpm/cur_del_bmp.xpm"
#include "xpm/cur_del_mask.xpm"
#include "xpm/cur_select_bmp.xpm"
#include "xpm/cur_select_mask.xpm"
#include "xpm/cur_hand_bmp.xpm"
#include "xpm/cur_hand_mask.xpm"
#endif


#define QG_SCROLLMARGIN 400


/**
 * Constructor.
 */
QG_GraphicView::QG_GraphicView(QWidget* parent, const char* name, WFlags f)
        : QWidget(parent, name, f), RS_GraphicView() {

    setBackground(background);
    buffer = NULL;
	lastWidth = 0;
	lastHeight = 0;
    //coordinateWidget = NULL;
    //mouseWidget = NULL;
    //optionWidget = NULL;
    //cadToolBar = NULL;
    //commandWidget = NULL;

    layout = new QGridLayout(this, 3, 2);
    layout->setColStretch(0, 1);
    layout->setColStretch(1, 0);
    layout->setColStretch(2, 0);
    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 0);

    hScrollBar = new QG_ScrollBar(Horizontal, this);
    hScrollBar->setLineStep(50);
    layout->addMultiCellWidget(hScrollBar, 1, 1, 0, 0);
    layout->addRowSpacing(1, hScrollBar->sizeHint().height());
    connect(hScrollBar, SIGNAL(valueChanged(int)),
            this, SLOT(slotHScrolled(int)));

    vScrollBar = new QG_ScrollBar(Vertical, this);
    vScrollBar->setLineStep(50);
    layout->addMultiCellWidget(vScrollBar, 0, 0, 2, 2);
    layout->addColSpacing(2, vScrollBar->sizeHint().width());
    connect(vScrollBar, SIGNAL(valueChanged(int)),
            this, SLOT(slotVScrolled(int)));

#ifndef __APPLE__
    // Mouse Cursors:
    QBitmap bmp;
    QPixmap cur1(cur_cad_bmp_xpm);
    bmp = QPixmap(cur_cad_mask_xpm);
    cur1.setMask(bmp);
    curCad = new QCursor(cur1, 15, 15);

    QPixmap cur2(cur_glass_bmp_xpm);
    bmp = QPixmap(cur_glass_mask_xpm);
    cur2.setMask(bmp);
    curMagnifier = new QCursor(cur2, 12, 12);

    QPixmap cur3(cur_del_bmp_xpm);
    bmp = QPixmap(cur_del_mask_xpm);
    cur3.setMask(bmp);
    curDel = new QCursor(cur3, 15, 15);

    QPixmap cur4(cur_select_bmp_xpm);
    bmp = QPixmap(cur_select_mask_xpm);
    cur4.setMask(bmp);
    curSelect = new QCursor(cur4, 15, 15);

    QPixmap cur5(cur_hand_bmp_xpm);
    bmp = QPixmap(cur_hand_mask_xpm);
    cur5.setMask(bmp);
    curHand = new QCursor(cur5, 15, 15);
#else
    // No individual cursors for the Mac
    curCad = NULL;
    curMagnifier = NULL;
    curDel = NULL;
    curSelect = NULL;
    curHand = NULL;
#endif

    // Dummy widgets for scrollbar corners:
    //layout->addWidget(new QWidget(this), 1, 1);
    //QWidget* w = new QWidget(this);
    //w->setEraseColor(QColor(255,0,0));
    gridStatus = new QLabel("-", this);
    gridStatus->setAlignment(Qt::AlignRight);
    layout->addMultiCellWidget(gridStatus, 1, 1, 1, 2);
    layout->addColSpacing(1, 50);

	
    setMouseTracking(true);
	// flickering under win:
    //setFocusPolicy(WheelFocus);
	
    setFocusPolicy(NoFocus);
}



/**
 * Destructor
 */
QG_GraphicView::~QG_GraphicView() {
	if (painter!=NULL) {
		((RS_PainterQt*)painter)->end();
		delete painter;
	}
	if (buffer!=NULL) {
		delete buffer;
		buffer = NULL;
	}
    cleanUp();
}



/**
 * @return width of widget.
 */
int QG_GraphicView::getWidth() {
    return width() - vScrollBar->sizeHint().width();
}



/**
 * @return height of widget.
 */
int QG_GraphicView::getHeight() {
    return height() - hScrollBar->sizeHint().height();
}



/**
 * Creates a new painter for the buffer of this widget and returns a 
 * pointer to it. The class variable 'painter' also
 * points to that object.
 */
RS_Painter* QG_GraphicView::createPainter() {
    RS_DEBUG->print("QG_GraphicView::createPainter begin");

	if (lastWidth!=getWidth() || lastHeight!=getHeight()) {
		destroyPainter();
		if (buffer!=NULL) {
			delete buffer;
			buffer = NULL;
		}
		lastWidth = getWidth();
		lastHeight = getHeight();
	}

    if (buffer==NULL) {
        //RS_DEBUG->timestamp();
        RS_DEBUG->print("creating buffer: %d,%d", getWidth(), getHeight());
        buffer = new QPixmap(getWidth(), getHeight());
        RS_DEBUG->print("ok");
	}
	
    if (painter==NULL) {
		painter = new RS_PainterQt(buffer);
		painter->setDrawingMode(drawingMode);
		((RS_PainterQt*)painter)->setBackgroundMode(Qt::OpaqueMode);
		((RS_PainterQt*)painter)->setBackgroundColor(background);
		((RS_PainterQt*)painter)->eraseRect(0,0,getWidth(), getHeight());
		
        //RS_DEBUG->timestamp();
    }
    //if (painter!=NULL) {
    //    delete painter;
    //}

    RS_DEBUG->print("QG_GraphicView::createPainter end");

    return painter;
}



/**
 * Creates a new painter for this widget and returns a 
 * pointer to it. The class variable 'painter' also
 * automatically points to that object.
 */
RS_Painter* QG_GraphicView::createDirectPainter() {

    RS_DEBUG->print("QG_GraphicView::createDirectPainter begin");

	destroyPainter();
    painter = new RS_PainterQt(this);
    painter->setDrawingMode(drawingMode);

    RS_DEBUG->print("QG_GraphicView::createDirectPainter end");

    return painter;
}



/**
 * Deletes the painter.
 */
void QG_GraphicView::destroyPainter() {

    RS_DEBUG->print("QG_GraphicView::destroyPainter begin");

    if (painter!=NULL) {
        delete painter;
        painter = NULL;
    }

    RS_DEBUG->print("QG_GraphicView::destroyPainter end");
}



/**
 * Changes the current background color of this view.
 */
void QG_GraphicView::setBackground(const RS_Color& bg) {
    RS_GraphicView::setBackground(bg);

    setBackgroundColor(bg);
}



/**
 * Sets the mouse cursor to the given type.
 */
void QG_GraphicView::setMouseCursor(RS2::CursorType c) {

#if QT_VERSION>=0x030000
    switch (c) {
    default:
    case RS2::ArrowCursor:
        setCursor(Qt::ArrowCursor);
        break;
    case RS2::UpArrowCursor:
        setCursor(Qt::UpArrowCursor);
        break;
    case RS2::CrossCursor:
        setCursor(Qt::CrossCursor);
        break;
    case RS2::WaitCursor:
        setCursor(Qt::WaitCursor);
        break;
    case RS2::IbeamCursor:
        setCursor(Qt::IbeamCursor);
        break;
    case RS2::SizeVerCursor:
        setCursor(Qt::SizeVerCursor);
        break;
    case RS2::SizeHorCursor:
        setCursor(Qt::SizeHorCursor);
        break;
    case RS2::SizeBDiagCursor:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case RS2::SizeFDiagCursor:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case RS2::SizeAllCursor:
        setCursor(Qt::SizeAllCursor);
        break;
    case RS2::BlankCursor:
        setCursor(Qt::BlankCursor);
        break;
    case RS2::SplitVCursor:
        setCursor(Qt::SplitVCursor);
        break;
    case RS2::SplitHCursor:
        setCursor(Qt::SplitHCursor);
        break;
    case RS2::PointingHandCursor:
        setCursor(Qt::PointingHandCursor);
        break;
    case RS2::ForbiddenCursor:
        setCursor(Qt::ForbiddenCursor);
        break;
    case RS2::WhatsThisCursor:
        setCursor(Qt::WhatsThisCursor);
        break;

#ifndef __APPLE__

    case RS2::CadCursor:
        setCursor(*curCad);
        break;
    case RS2::DelCursor:
        setCursor(*curDel);
        break;
    case RS2::SelectCursor:
        setCursor(*curSelect);
        break;
    case RS2::MagnifierCursor:
        setCursor(*curMagnifier);
        break;
    case RS2::MovingHandCursor:
        setCursor(*curHand);
        break;
#else
        // Reduced cursor selection for the Mac:
    case RS2::CadCursor:
        setCursor(Qt::CrossCursor);
        break;
    case RS2::DelCursor:
        setCursor(Qt::CrossCursor);
        break;
    case RS2::SelectCursor:
        setCursor(Qt::CrossCursor);
        break;
    case RS2::MagnifierCursor:
        setCursor(Qt::CrossCursor);
        break;
    case RS2::MovingHandCursor:
        setCursor(Qt::PointingHandCursor);
        break;
#endif
    }

#endif
}



/**
 * Sets the text for the grid status widget in the left bottom corner.
 */
void QG_GraphicView::updateGridStatusWidget(const RS_String& text) {
    gridStatus->setText(text);
}



/**
 * Redraws the widget.
 */
void QG_GraphicView::redraw() {

    RS_DEBUG->print("QG_GraphicView::redraw begin 1");
	if (simulationRunning) {
		return;
	}

	// never run twice
    static bool running = false;

    if (!running) {
        running = true;

        RS_DEBUG->print("QG_GraphicView::redraw begin 2");

        if (isUpdateEnabled()) {
            repaint(false);
        }

        RS_DEBUG->print("QG_GraphicView::redraw end 2");
        running = false;
    }
    RS_DEBUG->print("QG_GraphicView::redraw end 1");
}



void QG_GraphicView::resizeEvent(QResizeEvent* /*e*/) {
    RS_DEBUG->print("QG_GraphicView::resizeEvent begin");
    adjustOffsetControls();
    adjustZoomControls();
    updateGrid();
    RS_DEBUG->print("QG_GraphicView::resizeEvent end");
}



void QG_GraphicView::emulateMouseMoveEvent() {
    RS_MouseEvent e(QEvent::MouseMove, QPoint(mx, my),
                    Qt::NoButton, Qt::NoButton);
    //mouseMoveEvent(&e);
}



void QG_GraphicView::mousePressEvent(QMouseEvent* e) {
    // pan zoom with middle mouse button
    if (e->button()==MidButton /*|| (e->state()==Qt::LeftButton|Qt::AltButton)*/) {
        setCurrentAction(new RS_ActionZoomPan(*container, *this));
    }

    RS_GraphicView::mousePressEvent(e);
    QWidget::mousePressEvent(e);
}


void QG_GraphicView::mouseReleaseEvent(QMouseEvent* e) {
	RS_DEBUG->print("QG_GraphicView::mouseReleaseEvent");
    RS_GraphicView::mouseReleaseEvent(e);
    //QWidget::mouseReleaseEvent(e);

#if QT_VERSION>=0x030000
    if (!e->isAccepted()) {
        if (QG_DIALOGFACTORY!=NULL && QG_DIALOGFACTORY->getCadToolBar()!=NULL) {
			RS_DEBUG->print("QG_GraphicView::mouseReleaseEvent: "
				"fwd to cadtoolbar");
            QG_DIALOGFACTORY->getCadToolBar()->mouseReleaseEvent(e);
        }
    }
#endif
	RS_DEBUG->print("QG_GraphicView::mouseReleaseEvent: OK");
}


void QG_GraphicView::mouseMoveEvent(QMouseEvent* e) {
    //RS_DEBUG->print("QG_GraphicView::mouseMoveEvent begin");
    //RS_MouseEvent rsm = QG_Qt2Rs::mouseEvent(e);

    RS_GraphicView::mouseMoveEvent(e);
    QWidget::mouseMoveEvent(e);

#ifdef Q_OS_WIN32
	// make sure that we can still use hotkeys and the mouse wheel
	if (parent()!=NULL) {
		((QWidget*)parent())->setFocus();
	}
#endif
	
    //RS_DEBUG->print("QG_GraphicView::mouseMoveEvent end");
}


/**
 * support for the wacom graphic tablet.
 */
#if QT_VERSION>=0x030000
void QG_GraphicView::tabletEvent(QTabletEvent* e) {
    if (hasMouse()) {
        switch (e->device()) {
        case QTabletEvent::Eraser:
            if (e->type()==QEvent::TabletRelease) {
                if (container!=NULL) {

                    RS_ActionSelectSingle* a =
                        new RS_ActionSelectSingle(*container, *this);
                    setCurrentAction(a);
                    QMouseEvent ev(QEvent::MouseButtonRelease, e->pos(),
                                   Qt::LeftButton, Qt::LeftButton);
                    mouseReleaseEvent(&ev);
                    a->finish();

                    if (container->countSelected()>0) {
                        setCurrentAction(
                            new RS_ActionModifyDelete(*container, *this));
                    }
                }
            }
            break;

        case QTabletEvent::Stylus:
        case QTabletEvent::Puck:
            if (e->type()==QEvent::TabletPress) {
                QMouseEvent ev(QEvent::MouseButtonPress, e->pos(),
                               Qt::LeftButton, Qt::LeftButton);
                mousePressEvent(&ev);
            } else if (e->type()==QEvent::TabletRelease) {
                QMouseEvent ev(QEvent::MouseButtonRelease, e->pos(),
                               Qt::LeftButton, Qt::LeftButton);
                mouseReleaseEvent(&ev);
            } else if (e->type()==QEvent::TabletMove) {
                QMouseEvent ev(QEvent::MouseMove, e->pos(),
                               Qt::NoButton, 0);
                mouseMoveEvent(&ev);
            }
            break;

        default:
            break;
        }
    }

    // a 'mouse' click:
    /*if (e->pressure()>10 && lastPressure<10) {
    	QMouseEvent e(QEvent::MouseButtonPress, e->pos(), 
    	   Qt::LeftButton, Qt::LeftButton);
    	mousePressEvent(&e);
}
    else if (e->pressure()<10 && lastPressure>10) {
    	QMouseEvent e(QEvent::MouseButtonRelease, e->pos(), 
    	   Qt::LeftButton, Qt::LeftButton);
    	mouseReleaseEvent(&e);
}	else if (lastPos!=e->pos()) {
    	QMouseEvent e(QEvent::MouseMove, e->pos(), 
    	   Qt::NoButton, 0);
    	mouseMoveEvent(&e);
}

    lastPressure = e->pressure();
    lastPos = e->pos();
    */
}
#endif

void QG_GraphicView::leaveEvent(QEvent* e) {
    RS_GraphicView::mouseLeaveEvent();
    QWidget::leaveEvent(e);
}


void QG_GraphicView::enterEvent(QEvent* e) {
    RS_GraphicView::mouseEnterEvent();
    QWidget::enterEvent(e);
}


void QG_GraphicView::focusOutEvent(QFocusEvent* e) {
    QWidget::focusOutEvent(e);
}


void QG_GraphicView::focusInEvent(QFocusEvent* e) {
    RS_GraphicView::mouseEnterEvent();
    QWidget::focusInEvent(e);
}


/**
 * mouse wheel event. zooms in/out or scrolls when
 * shift or ctrl is pressed.
 */
void QG_GraphicView::wheelEvent(QWheelEvent *e) {
    //RS_DEBUG->print("wheel: %d", e->delta());

    //printf("state: %d\n", e->state());
    //printf("ctrl: %d\n", Qt::ControlButton);

    if (container==NULL) {
        return;
    }

	RS_Vector mouse = toGraph(RS_Vector(e->x(), e->y()));

    bool scroll = false;
    RS2::Direction direction = RS2::Up;

    // scroll up / down:
    if (e->state()==Qt::ControlButton) {
        scroll = true;
        if (e->delta()>0) {
            direction = RS2::Up;
        } else {
            direction = RS2::Down;
        }
    }

    // scroll left / right:
    else if	(e->state()==Qt::ShiftButton) {
        scroll = true;
        if (e->delta()>0) {
            direction = RS2::Right;
        } else {
            direction = RS2::Left;
        }
    }

    if (scroll) {
        setCurrentAction(new RS_ActionZoomScroll(direction,
                         *container, *this));
    }

    // zoom in / out:
    else if (e->state()==0) {
        if (e->delta()>0) {
            setCurrentAction(new RS_ActionZoomIn(*container, *this,
                                                 RS2::In, RS2::Both,
												 mouse));
        } else {
            setCurrentAction(new RS_ActionZoomIn(*container, *this,
                                                 RS2::Out, RS2::Both,
												 mouse));
        }
    }

    e->accept();
}


void QG_GraphicView::keyPressEvent(QKeyEvent* e) {
    //if (e->key()==Qt::Key_Control) {
    //	setCtrlPressed(true);
    //}


    if (container==NULL) {
        return;
    }

    bool scroll = false;
    RS2::Direction direction = RS2::Up;

    switch (e->key()) {
    case Qt::Key_Left:
        scroll = true;
        direction = RS2::Right;
        break;
    case Qt::Key_Right:
        scroll = true;
        direction = RS2::Left;
        break;
    case Qt::Key_Up:
        scroll = true;
        direction = RS2::Up;
        break;
    case Qt::Key_Down:
        scroll = true;
        direction = RS2::Down;
        break;
    default:
        scroll = false;
        break;
    }

    if (scroll) {
        setCurrentAction(new RS_ActionZoomScroll(direction,
                         *container, *this));
    }

    RS_GraphicView::keyPressEvent(e);
}


void QG_GraphicView::keyReleaseEvent(QKeyEvent* e) {
    //if (e->key()==Qt::Key_Control) {
    //	setCtrlPressed(false);
    //}
    RS_GraphicView::keyReleaseEvent(e);
}


/**
 * Called whenever the graphic view has changed.
 * Adjusts the scrollbar ranges / steps.
 */
void QG_GraphicView::adjustOffsetControls() {
	static bool running = false;

	if (running) {
		return;
	}

	running = true;

    RS_DEBUG->print("QG_GraphicView::adjustOffsetControls() begin");

    if (container==NULL || hScrollBar==NULL || vScrollBar==NULL) {
        return;
    }

    disableUpdate();
    int ox = getOffsetX();
    int oy = getOffsetY();
	
    RS_Vector min = container->getMin();
    RS_Vector max = container->getMax();

    // no drawing yet - still allow to scroll
    if (max.x < min.x+1.0e-6 || 
	    max.y < min.y+1.0e-6 ||
		max.x > RS_MAXDOUBLE || 
		max.x < RS_MINDOUBLE || 
		min.x > RS_MAXDOUBLE || 
		min.x < RS_MINDOUBLE ||
		max.y > RS_MAXDOUBLE || 
		max.y < RS_MINDOUBLE || 
		min.y > RS_MAXDOUBLE || 
		min.y < RS_MINDOUBLE ) {
        min = RS_Vector(-10,-10);
        max = RS_Vector(100,100);
    }
	
	int minVal = (int)(min.x * getFactor().x 
			- QG_SCROLLMARGIN - getBorderLeft());
	int maxVal = (int)(max.x * getFactor().x 
			- getWidth() + QG_SCROLLMARGIN + getBorderRight());

	hScrollBar->setValue(0);
	if (minVal<=maxVal) {
		hScrollBar->setRange(minVal, maxVal);
	}
    //hScrollBar->setMinValue(minVal);
    
	//hScrollBar->setMaxValue(maxVal);

	minVal = (int)(getHeight() - max.y * getFactor().y 
			- QG_SCROLLMARGIN - getBorderTop());
	maxVal = (int)(QG_SCROLLMARGIN + getBorderBottom() 
			- (min.y * getFactor().y));

	if (minVal<=maxVal) {
		vScrollBar->setRange(minVal, maxVal);
	}
    //vScrollBar->setMaxValue((int)(QG_SCROLLMARGIN + getBorderBottom()
     //                             - (min.y * getFactor().y)));
								  
	
    //vScrollBar->setMinValue((int)(getHeight() -
     //                             max.y * getFactor().y
     //                             - QG_SCROLLMARGIN - getBorderTop()));
								  

    hScrollBar->setPageStep((int)(getWidth()));
    vScrollBar->setPageStep((int)(getHeight()));

    hScrollBar->setValue(-ox);
    vScrollBar->setValue(oy);
	

    slotHScrolled(-ox);
    slotVScrolled(oy);
	

    RS_DEBUG->print("H min: %d / max: %d / step: %d / value: %d\n",
                    hScrollBar->minValue(), hScrollBar->maxValue(),
                    hScrollBar->pageStep(), ox);
    RS_DEBUG->print("V min: %d / max: %d / step: %d / value: %d\n",
                    vScrollBar->minValue(), vScrollBar->maxValue(),
                    vScrollBar->pageStep(), oy);

    enableUpdate();

    RS_DEBUG->print("QG_GraphicView::adjustOffsetControls() end");

	running = false;
}


/**
 * override this to adjust controls and widgets that
 * control the zoom factor of the graphic.
 */
void QG_GraphicView::adjustZoomControls() {}


/**
 * Slot for horizontal scroll events.
 */
void QG_GraphicView::slotHScrolled(int value) {
    // Scrollbar behaviour tends to change with every Qt version..
    // so let's keep old code in here for now

    //static int running = false;
    //if (!running) {
    //running = true;
    ////RS_DEBUG->print("value x: %d\n", value);
    if (hScrollBar->maxValue()==hScrollBar->minValue()) {
        centerOffsetX();
    } else {
        setOffsetX(-value);
    }
    //if (isUpdateEnabled()) {
    updateGrid();
    redraw();
    //}
    ////updateView();
    //running = false;
    //}
}


/**
 * Slot for vertical scroll events.
 */
void QG_GraphicView::slotVScrolled(int value) {
    // Scrollbar behaviour tends to change with every Qt version..
    // so let's keep old code in here for now

    //static int running = false;
    //if (!running) {
    //running = true;
    ////RS_DEBUG->print("value y: %d\n", value);
    if (vScrollBar->maxValue()==vScrollBar->minValue()) {
        centerOffsetY();
    } else {
        setOffsetY(value);
    }
    //if (isUpdateEnabled()) {
    updateGrid();
    redraw();
    //}
    ////updateView();
    //running = false;
    //}
}


/**
 * Handles paint events by redrawing the graphic in this view.
 * usually that's very fast since we only paint the buffer we
 * have from the last call..
 */
void QG_GraphicView::paintEvent(QPaintEvent *) {
    RS_DEBUG->print("QG_GraphicView::paintEvent begin");
	if (simulationRunning) {
		return;
	}
    drawIt();

    if (buffer!=NULL) {
        QPainter wPainter;
        wPainter.begin(this);
        wPainter.drawPixmap(0,0,*buffer);
        wPainter.end();
    }

    //if (buffer!=NULL) {
    //    delete buffer;
    //    buffer = NULL;
    //}
    RS_DEBUG->print("QG_GraphicView::paintEvent end");
}



/**
 * Previews the given url for the file open dialog.
 */
void QG_GraphicView::previewUrl(const QUrl &u) {
	//static RS_Graphic* gr = new RS_Graphic();
	
	RS_DEBUG->print("QG_GraphicView::previewUrl");
	
	if (container!=NULL && container->rtti()==RS2::EntityGraphic) {
		((RS_Graphic*)container)->open(u.path(), RS2::FormatUnknown);
		zoomAuto();
	}
	//setContainer(gr);
	
	RS_DEBUG->print("QG_GraphicView::previewUrl: OK");
}


