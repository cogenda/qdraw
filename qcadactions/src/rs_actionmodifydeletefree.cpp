/****************************************************************************
** $Id: rs_actionmodifydeletefree.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionmodifydeletefree.h"

#include "rs_point.h"
#include "rs_polyline.h"
#include "rs_modification.h"



RS_ActionModifyDeleteFree::RS_ActionModifyDeleteFree(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionInterface("Delete Entities Freehand",
                    container, graphicView) {}

QAction* RS_ActionModifyDeleteFree::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Delete Freehand"), tr("&Delete Freehand"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Delete Freehand"));
    return action;
}


void RS_ActionModifyDeleteFree::init(int status) {
    RS_ActionInterface::init(status);
    polyline = NULL;
    e1 = e2 = NULL;
    v1 = v2 = RS_Vector(false);
    setSnapMode(RS2::SnapOnEntity);
}



void RS_ActionModifyDeleteFree::trigger() {
    if (e1!=NULL && e2!=NULL) {
        RS_EntityContainer* parent = e2->getParent();
        if (parent!=NULL) {
            if (parent->rtti()==RS2::EntityPolyline) {
                if(parent->getId() == polyline->getId()) {

                    // deletes whole polyline on screen:
                    graphicView->deleteEntity((RS_Entity*)polyline);

                    // splits up the polyline in the container:
                    RS_Polyline* pl1;
                    RS_Polyline* pl2;
                    RS_Modification m(*container);
                    m.splitPolyline(*polyline,
                                    *e1, v1,
                                    *e2, v2,
                                    &pl1, &pl2);

                    if (document) {
                        document->startUndoCycle();
                        document->addUndoable(polyline);
                        document->addUndoable(pl1);
                        document->addUndoable(pl2);
                        document->endUndoCycle();
                    }

                    // draws the new polylines on the screen:
                    graphicView->drawEntity((RS_Entity*)pl1);
                    graphicView->drawEntity((RS_Entity*)pl2);

                    init();

                    RS_DIALOGFACTORY->updateSelectionWidget(
                        container->countSelected());
                } else {
        			RS_DIALOGFACTORY->commandMessage(tr("Entities not in the same polyline."));
                }
            } else {
        		RS_DIALOGFACTORY->commandMessage(tr("Parent of second entity is not a polyline"));
            }
        } else {
        	RS_DIALOGFACTORY->commandMessage(tr("Parent of second entity is NULL"));
        }
    } else {
        RS_DIALOGFACTORY->commandMessage(tr("One of the chosen entities is NULL"));
    }
}



void RS_ActionModifyDeleteFree::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    } else {

        switch (getStatus()) {
        case 0: {
                v1 = snapPoint(e);
                e1 = getKeyEntity();
                if (e1!=NULL) {
                    RS_EntityContainer* parent = e1->getParent();
                    if (parent!=NULL) {
                        if (parent->rtti()==RS2::EntityPolyline) {
                            polyline = (RS_Polyline*)parent;
                            setStatus(1);
                        } else {
        					RS_DIALOGFACTORY->commandMessage(
								tr("Parent of first entity is not a polyline"));
                        }
                    } else {
        				RS_DIALOGFACTORY->commandMessage(
							tr("Parent of first entity is NULL"));
                    }
                } else {
        			RS_DIALOGFACTORY->commandMessage(
						tr("First entity is NULL"));
                }
            }
            break;

        case 1: {
                v2 = snapPoint(e);
                e2 = getKeyEntity();

                if (e2!=NULL) {
                    trigger();
                } else {
        			RS_DIALOGFACTORY->commandMessage(tr("Second entity is NULL"));
                }
            }
            break;
        }
    }
}



void RS_ActionModifyDeleteFree::updateMouseButtonHints() {
    switch (getStatus()) {
    case 0:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify first break point "
                                               "on a polyline"), tr("Cancel"));
        break;
    case 1:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify second break point "
                                               "on the same polyline"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}

// EOF
