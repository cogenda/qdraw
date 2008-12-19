/****************************************************************************
** $Id: rs_actiondrawspline.cpp 1140 2004-07-24 23:14:15Z andrew $
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

#include "rs_actiondrawspline.h"
#include "rs_actioneditundo.h"
#include "rs_snapper.h"



RS_ActionDrawSpline::RS_ActionDrawSpline(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw splines",
                           container, graphicView) {

    reset();
    history.setAutoDelete(true);
    data = RS_SplineData(3, false);
    //bHistory.setAutoDelete(true);
}



RS_ActionDrawSpline::~RS_ActionDrawSpline() {}


QAction* RS_ActionDrawSpline::createGUIAction(RS2::ActionType /*type*/, 
				QObject* /*parent*/) {
    QAction* action = new QAction(tr("Spline"), tr("&Spline"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Draw splines"));
    return action;
}



void RS_ActionDrawSpline::reset() {
	spline = NULL;
    //start = RS_Vector(false);
    history.clear();
    //bHistory.clear();
}



void RS_ActionDrawSpline::init(int status) {
    RS_PreviewActionInterface::init(status);

    reset();
}



void RS_ActionDrawSpline::trigger() {
    RS_PreviewActionInterface::trigger();

	if (spline==NULL) {
		return;
	}

	// add the entity
    //RS_Spline* spline = new RS_Spline(container, data);
    spline->setLayerToActive();
    spline->setPenToActive();
	spline->update();
    container->addEntity(spline);

    // upd. undo list:
    if (document!=NULL) {
        document->startUndoCycle();
        document->addUndoable(spline);
        document->endUndoCycle();
    }

	// upd view
    deleteSnapper();
	RS_Vector r = graphicView->getRelativeZero();
    graphicView->moveRelativeZero(RS_Vector(0.0,0.0));
    graphicView->drawEntity(spline);
    graphicView->moveRelativeZero(r);
    drawSnapper();
    RS_DEBUG->print("RS_ActionDrawSpline::trigger(): spline added: %d",
                    spline->getId());

	spline = NULL;
    //history.clear();
}



void RS_ActionDrawSpline::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawSpline::mouseMoveEvent begin");

    RS_Vector mouse = snapPoint(e);
    if (getStatus()==SetNextPoint && spline!=NULL /*&& point.valid*/) {
        deletePreview();
        clearPreview();
		
		RS_Spline* tmpSpline = (RS_Spline*)spline->clone();
		tmpSpline->addControlPoint(mouse);
		tmpSpline->update();
		preview->addEntity(tmpSpline);
		
		RS_ValueList<RS_Vector> cpts = tmpSpline->getControlPoints();
		RS_ValueList<RS_Vector>::iterator it;
		for (it=cpts.begin(); it!=cpts.end(); ++it) {
			preview->addEntity(new RS_Point(preview, RS_PointData(*it)));
		}
        drawPreview();
    }

    RS_DEBUG->print("RS_ActionDrawSpline::mouseMoveEvent end");
}



void RS_ActionDrawSpline::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
		if (getStatus()==SetNextPoint) {
			trigger();
		}
        deletePreview();
        clearPreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionDrawSpline::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetStartpoint:
		//data.startpoint = mouse;
        //point = mouse;
        history.clear();
        history.append(new RS_Vector(mouse));
		if (spline==NULL) {
			spline = new RS_Spline(container, data);
			spline->addControlPoint(mouse);
		}
        //bHistory.clear();
        //bHistory.append(new double(0.0));
        //start = mouse;
        setStatus(SetNextPoint);
        graphicView->moveRelativeZero(mouse);
        updateMouseButtonHints();
        break;

    case SetNextPoint:
    	graphicView->moveRelativeZero(mouse);
        //point = mouse;
        history.append(new RS_Vector(mouse));
        //bHistory.append(new double(0.0));
		if (spline!=NULL) {
			//graphicView->deleteEntity(spline);
			spline->addControlPoint(mouse);
			//spline->setEndpoint(mouse);
			//if (spline->count()==1) {
    			//spline->setLayerToActive();
    			//spline->setPenToActive();
				//container->addEntity(spline);
			//}
			deletePreview();
			clearPreview();
			deleteSnapper();
			//graphicView->drawEntity(spline);
			drawSnapper();
		}
        //trigger();
        //data.startpoint = data.endpoint;
        updateMouseButtonHints();
        //graphicView->moveRelativeZero(mouse);
        break;

    default:
        break;
    }
}



void RS_ActionDrawSpline::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    switch (getStatus()) {
    case SetStartpoint:
        if (checkCommand("help", c)) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
            return;
        }
        break;

    case SetNextPoint:
        /*if (checkCommand("close", c)) {
            close();
            updateMouseButtonHints();
            return;
        }*/

        if (checkCommand("undo", c)) {
            undo();
            updateMouseButtonHints();
            return;
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawSpline::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetStartpoint:
        break;
    case SetNextPoint:
        if (history.count()>=2) {
            cmd += command("undo");
        }
        if (history.count()>=3) {
            cmd += command("close");
        }
        break;
    default:
        break;
    }

    return cmd;
}



void RS_ActionDrawSpline::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetStartpoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify first control point"),
                                            tr("Cancel"));
        break;
    case SetNextPoint: {
            RS_String msg = "";

            if (history.count()>=3) {
                msg += RS_COMMANDS->command("close");
                msg += "/";
            }
            if (history.count()>=2) {
                msg += RS_COMMANDS->command("undo");
            }

            if (history.count()>=2) {
                RS_DIALOGFACTORY->updateMouseWidget(
                    tr("Specify next control point or [%1]").arg(msg),
                    tr("Back"));
            } else {
                RS_DIALOGFACTORY->updateMouseWidget(
                    tr("Specify next control point"),
                    tr("Back"));
            }
        }
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}


void RS_ActionDrawSpline::showOptions() {
    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}



void RS_ActionDrawSpline::hideOptions() {
    RS_ActionInterface::hideOptions();

    RS_DIALOGFACTORY->requestOptions(this, false);
}


void RS_ActionDrawSpline::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}


void RS_ActionDrawSpline::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        //RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSplines);
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}


/*
void RS_ActionDrawSpline::close() {
    if (history.count()>2 && start.valid) {
        //data.endpoint = start;
        //trigger();
		if (spline!=NULL) {
			RS_CoordinateEvent e(spline->getStartpoint());
			coordinateEvent(&e);
		}
		trigger();
        setStatus(SetStartpoint);
        graphicView->moveRelativeZero(start);
    } else {
        RS_DIALOGFACTORY->commandMessage(
            tr("Cannot close sequence of lines: "
               "Not enough entities defined yet."));
    }
}
*/

void RS_ActionDrawSpline::undo() {
    if (history.count()>1) {
        history.removeLast();
        //bHistory.removeLast();
        deletePreview();
        clearPreview();
        //graphicView->setCurrentAction(
        //    new RS_ActionEditUndo(true, *container, *graphicView));
		if (history.last()!=NULL) {
        	//point = *history.last();
		}
		if (spline!=NULL) {
			spline->removeLastControlPoint();
			RS_Vector* v = history.last();
			if (v!=NULL) {
	        	graphicView->moveRelativeZero(*v);
			}
			graphicView->redraw();
		}
    } else {
        RS_DIALOGFACTORY->commandMessage(
            tr("Cannot undo: "
               "Not enough entities defined yet."));
    }
}


void RS_ActionDrawSpline::setDegree(int deg) {
	data.degree = deg;
	if (spline!=NULL) {
		spline->setDegree(deg);
	}
}

int RS_ActionDrawSpline::getDegree() {
	return data.degree;
}

void RS_ActionDrawSpline::setClosed(bool c) {
	data.closed = c;
	if (spline!=NULL) {
		spline->setClosed(c);
	}
}

bool RS_ActionDrawSpline::isClosed() {
	return data.closed;
}

// EOF
