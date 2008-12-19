/****************************************************************************
** $Id: rs_actionzoomin.cpp 1090 2004-03-16 10:02:59Z js $
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

#include "rs_actionzoomin.h"


/**
 * Default constructor.
 *
 * @param direction In for zooming in, Out for zooming out.
 * @param axis Axis that are affected by the zoom (OnlyX, OnlyY or Both)
 */
RS_ActionZoomIn::RS_ActionZoomIn(RS_EntityContainer& container,
                                 RS_GraphicView& graphicView,
                                 RS2::ZoomDirection direction,
                                 RS2::Axis axis,
                                 const RS_Vector& center)
        :RS_ActionInterface("Zoom in", container, graphicView) {

    this->direction = direction;
    this->axis = axis;
    this->center = center;
}

QAction* RS_ActionZoomIn::createGUIAction(RS2::ActionType type, QObject* /*parent*/) {
    QAction* action;

    if (type==RS2::ActionZoomIn) {
        action = new QAction(tr("Zoom in"), QPixmap::fromMimeSource("zoomin.png"),
                             tr("Zoom &In"), QKeySequence(), NULL);
        action->setStatusTip(tr("Zooms in"));
    } else {
        action = new QAction(tr("Zoom out"), QPixmap::fromMimeSource("zoomout.png"),
                             tr("Zoom &Out"), QKeySequence(), NULL);
        action->setStatusTip(tr("Zooms out"));
    }
    return action;
}


void RS_ActionZoomIn::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

void RS_ActionZoomIn::trigger() {
    switch (axis) {
    case RS2::OnlyX:
        if (direction==RS2::In) {
            graphicView->zoomInX();
        } else {
            graphicView->zoomOutX();
        }
        break;

    case RS2::OnlyY:
        if (direction==RS2::In) {
            graphicView->zoomInY();
        } else {
            graphicView->zoomOutY();
        }
        break;

    case RS2::Both:
        if (direction==RS2::In) {
            graphicView->zoomIn(1.25, center);
        } else {
            graphicView->zoomOut(1.25, center);
        }
        break;
    }
    finish();
}




// EOF
