/****************************************************************************
** $Id: rs_actionblocksinsert.cpp 1148 2004-09-11 21:38:11Z andrew $
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

#include "rs_actionblocksinsert.h"

#include "rs_creation.h"
#include "rs_commands.h"
#include "rs_modification.h"


/**
 * Constructor.
 */
RS_ActionBlocksInsert::RS_ActionBlocksInsert(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Blocks Insert",
                           container, graphicView) {}



RS_ActionBlocksInsert::~RS_ActionBlocksInsert() {}

QAction* RS_ActionBlocksInsert::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    QAction* action = new QAction(tr("Insert Block"), tr("&Insert Block"),
                                  QKeySequence(), NULL);
    action->setStatusTip(tr("Insert Block"));

    return action;
}

void RS_ActionBlocksInsert::init(int status) {
    RS_PreviewActionInterface::init(status);

    reset();

    if (graphic!=NULL) {
        block = graphic->getActiveBlock();
        if (block!=NULL) {
            data.name = block->getName();
        } else {
            finish();
        }
    }
}



void RS_ActionBlocksInsert::reset() {
    data = RS_InsertData("",
                         RS_Vector(0.0,0.0),
                         RS_Vector(1.0,1.0),
                         0.0,
                         1, 1,
                         RS_Vector(1.0,1.0),
                         NULL,
                         RS2::Update);
}



void RS_ActionBlocksInsert::trigger() {
    deleteSnapper();
    deletePreview();
    clearPreview();

    //RS_Modification m(*container, graphicView);
    //m.paste(data.insertionPoint);
    //std::cout << *RS_Clipboard::instance();

    if (block!=NULL) {
        RS_Creation creation(container, graphicView);
		data.updateMode = RS2::Update;
        creation.createInsert(data);
    }

    graphicView->redraw();
    //finish();
}


void RS_ActionBlocksInsert::mouseMoveEvent(RS_MouseEvent* e) {
    switch (getStatus()) {
    case SetTargetPoint:
        data.insertionPoint = snapPoint(e);

        if (block!=NULL) {
            deletePreview();
            clearPreview();
            //preview->addAllFrom(*block);
            //preview->move(data.insertionPoint);
            RS_Creation creation(preview, NULL, false);
			// Create insert as preview only
			data.updateMode = RS2::PreviewUpdate;
            creation.createInsert(data);
            drawPreview();
        }
        break;

    default:
        break;
    }
}



void RS_ActionBlocksInsert::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionBlocksInsert::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    data.insertionPoint = e->getCoordinate();
    trigger();
}



void RS_ActionBlocksInsert::commandEvent(RS_CommandEvent* e) {
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
        } else if (checkCommand("columns", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetColumns);
        } else if (checkCommand("rows", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetRows);
        } else if (checkCommand("columnspacing", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetColumnSpacing);
        } else if (checkCommand("rowspacing", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetRowSpacing);
        }
        break;

    case SetAngle: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.angle = RS_Math::deg2rad(a);
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

    case SetColumns: {
            bool ok;
            int cols = (int)RS_Math::eval(c, &ok);
            if (ok==true) {
                data.cols = cols;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

    case SetRows: {
            bool ok;
            int rows = (int)RS_Math::eval(c, &ok);
            if (ok==true) {
                data.rows = rows;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

    case SetColumnSpacing: {
            bool ok;
            double cs = (int)RS_Math::eval(c, &ok);
            if (ok==true) {
                data.spacing.x = cs;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

    case SetRowSpacing: {
            bool ok;
            int rs = (int)RS_Math::eval(c, &ok);
            if (ok==true) {
                data.spacing.y = rs;
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



RS_StringList RS_ActionBlocksInsert::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetTargetPoint:
        cmd += command("angle");
        cmd += command("factor");
        ;
        cmd += command("columns");
        cmd += command("rows");
        cmd += command("columnspacing");
        cmd += command("rowspacing");
        break;
    default:
        break;
    }

    return cmd;
}


void RS_ActionBlocksInsert::showOptions() {
    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}



void RS_ActionBlocksInsert::hideOptions() {
    RS_ActionInterface::hideOptions();

    RS_DIALOGFACTORY->requestOptions(this, false);
}


void RS_ActionBlocksInsert::updateMouseButtonHints() {
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
    case SetColumns:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter columns:"),
                                            "");
        break;
    case SetRows:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter rows:"),
                                            "");
        break;
    case SetColumnSpacing:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter column spacing:"),
                                            "");
        break;
    case SetRowSpacing:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter row spacing:"),
                                            "");
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionBlocksInsert::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionBlocksInsert::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}


// EOF
