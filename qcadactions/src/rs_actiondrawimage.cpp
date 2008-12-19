/****************************************************************************
** $Id: rs_actiondrawimage.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actiondrawimage.h"

#include "rs_creation.h"
#include "rs_commands.h"
#include "rs_modification.h"

/**
 * Constructor.
 */
RS_ActionDrawImage::RS_ActionDrawImage(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Image",
                           container, graphicView) {
}



RS_ActionDrawImage::~RS_ActionDrawImage() {}


QAction* RS_ActionDrawImage::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        QAction* action = new QAction(tr("Image"), tr("&Image"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Insert Image (Bitmap)"));
		return action;
}

void RS_ActionDrawImage::init(int status) {
    RS_PreviewActionInterface::init(status);

    reset();

    data.file = RS_DIALOGFACTORY->requestImageOpenDialog();
	graphicView->redraw();

	if (!data.file.isEmpty()) {
    	//std::cout << "file: " << data.file << "\n";

    	img = RS_Img(data.file);
    	setStatus(SetTargetPoint);
	}
	else {
		finish();
		updateToolBar();
        //RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
	}
}



void RS_ActionDrawImage::reset() {
    data = RS_ImageData(0,
                        RS_Vector(0.0,0.0),
                        RS_Vector(1.0,0.0),
                        RS_Vector(0.0,1.0),
                        RS_Vector(1.0,1.0),
                        "",
                        50, 50, 0);
}



void RS_ActionDrawImage::trigger() {
    deleteSnapper();
    deletePreview();
    clearPreview();

    if (!data.file.isEmpty()) {
        RS_Creation creation(container, graphicView);
        creation.createImage(data);
    }

    graphicView->redraw();
    //RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    finish();
	updateToolBar();
}


void RS_ActionDrawImage::mouseMoveEvent(RS_MouseEvent* e) {
    switch (getStatus()) {
    case SetTargetPoint:
        data.insertionPoint = snapPoint(e);

        deletePreview();
        clearPreview();
        //RS_Creation creation(preview, NULL, false);
        //creation.createInsert(data);
        RS_Line* line;
        line = new RS_Line(preview,
                           RS_LineData(RS_Vector(0, 0),
                                       RS_Vector(img.width(), 0)));
        preview->addEntity(line);
        line = new RS_Line(preview,
                           RS_LineData(RS_Vector(img.width(), 0),
                                       RS_Vector(img.width(), img.height())));
        preview->addEntity(line);
        line = new RS_Line(preview,
                           RS_LineData(RS_Vector(img.width(),
                                                 img.height()), RS_Vector(0, img.height())));
        preview->addEntity(line);
        line = new RS_Line(preview,
                           RS_LineData(RS_Vector(0, img.height()),
                                       RS_Vector(0, 0)));
        preview->addEntity(line);
        preview->scale(RS_Vector(0,0),
                       RS_Vector(data.uVector.magnitude(), data.uVector.magnitude()));
        preview->rotate(RS_Vector(0,0), data.uVector.angle());
        preview->move(data.insertionPoint);

        drawPreview();
        break;

    default:
        break;
    }
}



void RS_ActionDrawImage::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deleteSnapper();
        //init(getStatus()-1);
		finish();
    }
}



void RS_ActionDrawImage::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    data.insertionPoint = e->getCoordinate();
    trigger();
}



void RS_ActionDrawImage::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetTargetPoint:
        if (checkCommand("angle", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetAngle);
        } else if (checkCommand("factor", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetFactor);
        }
        break;

    case SetAngle: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                setAngle(RS_Math::deg2rad(a));
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

    case SetFactor: {
            bool ok;
            double f = RS_Math::eval(c, &ok);
            if (ok==true) {
                setFactor(f);
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawImage::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetTargetPoint:
        cmd += command("angle");
        cmd += command("factor");
        break;
    default:
        break;
    }

    return cmd;
}


void RS_ActionDrawImage::showOptions() {
    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}



void RS_ActionDrawImage::hideOptions() {
    RS_ActionInterface::hideOptions();

    RS_DIALOGFACTORY->requestOptions(this, false);
}


void RS_ActionDrawImage::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetTargetPoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Cancel"));
        break;
    case SetAngle:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter angle:"),
                                            "");
        break;
    case SetFactor:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter factor:"),
                                            "");
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDrawImage::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawImage::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}


// EOF
