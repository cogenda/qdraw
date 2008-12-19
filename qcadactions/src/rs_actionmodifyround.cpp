/****************************************************************************
** $Id: rs_actionmodifyround.cpp 1161 2004-12-09 23:10:09Z andrew $
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

#include "rs_actionmodifyround.h"

#include "rs_information.h"
#include "rs_snapper.h"


RS_ActionModifyRound::RS_ActionModifyRound(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Round Entities",
                           container, graphicView) {

    entity1 = NULL;
    entity2 = NULL;
    coord1 = RS_Vector(false);
    coord2 = RS_Vector(false);
}

QAction* RS_ActionModifyRound::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        QAction* action = new QAction(tr("Round"), tr("&Round"),
                             QKeySequence(), NULL);
        action->setStatusTip(tr("Round Entities"));
		return action;
}


void RS_ActionModifyRound::init(int status) {
    RS_ActionInterface::init(status);

    snapMode = RS2::SnapFree;
    snapRes = RS2::RestrictNothing;
}



void RS_ActionModifyRound::trigger() {

    RS_DEBUG->print("RS_ActionModifyRound::trigger()");

    if (entity1!=NULL && entity1->isAtomic() &&
            entity2!=NULL && entity2->isAtomic()) {

        deletePreview();

        RS_Modification m(*container, graphicView);
        m.round(coord2,
                coord1,
                (RS_AtomicEntity*)entity1,
                coord2,
                (RS_AtomicEntity*)entity2,
                data);

        //coord = RS_Vector(false);
        coord1 = RS_Vector(false);
        entity1 = NULL;
        coord2 = RS_Vector(false);
        entity2 = NULL;
        setStatus(SetEntity1);

        clearPreview();

        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
        }
    }
}



void RS_ActionModifyRound::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyRound::mouseMoveEvent begin");

    RS_Vector mouse = graphicView->toGraph(e->x(), e->y());
    RS_Entity* se = catchEntity(e, RS2::ResolveAll);

    switch (getStatus()) {
    case SetEntity1:
        entity1 = se;
        coord1 = mouse;
        break;

    case SetEntity2:
        entity2 = se;
        coord2 = mouse;

        if (entity1!=NULL && entity2!=NULL && entity2->isAtomic() &&
			RS_Information::isTrimmable(entity1, entity2)) {

            deletePreview();
            clearPreview();
            //preview->addSelectionFrom(*container);
            //preview->move(targetPoint-referencePoint);
            RS_Entity* tmp1 = entity1->clone();
            RS_Entity* tmp2 = entity2->clone();
            tmp1->reparent(preview);
            tmp2->reparent(preview);
            preview->addEntity(tmp1);
            preview->addEntity(tmp2);

            bool trim = data.trim;
            data.trim = false;
            RS_Modification m(*preview, NULL, false);
            m.round(coord2,
                    coord1,
                    (RS_AtomicEntity*)tmp1,
                    coord2,
                    (RS_AtomicEntity*)tmp2,
                    data);
            data.trim = trim;

            preview->removeEntity(tmp1);
            preview->removeEntity(tmp2);
            drawPreview();
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionModifyRound::mouseMoveEvent end");
}



void RS_ActionModifyRound::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_Vector mouse = graphicView->toGraph(e->x(), e->y());
    RS_Entity* se = catchEntity(e, RS2::ResolveAll);

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case SetEntity1:
            entity1 = se;
            coord1 = mouse;
            if (entity1!=NULL && entity1->isAtomic() && 
				RS_Information::isTrimmable(entity1)) {
                setStatus(SetEntity2);
            }
            break;

        case SetEntity2:
            entity2 = se;
            coord2 = mouse;
            if (entity2!=NULL && entity2->isAtomic() &&
			    RS_Information::isTrimmable(entity1, entity2)) {
                //setStatus(ChooseRounding);
                trigger();
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionModifyRound::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    switch (getStatus()) {
    case SetEntity1:
    case SetEntity2:
        if (checkCommand("radius", c)) {
            deleteSnapper();
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetRadius);
        } else if (checkCommand("trim", c)) {
            /*deleteSnapper();
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetTrim);
            */
            data.trim = !data.trim;
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->requestOptions(this, true, true);
            }
        }
        break;

    case SetRadius: {
            bool ok;
            double r = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.radius = r;
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                }
            }
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->requestOptions(this, true, true);
            }
            setStatus(lastStatus);
        }
        break;

        /*case SetTrim: {
        if (c==cmdYes.lower() || c==cmdYes2) {
        data.trim = true;
    } else if (c==cmdNo.lower() || c==cmdNo2) {
        data.trim = false;
                } else {
                    RS_DIALOGFACTORY->commandMessage(tr("Please enter 'Yes' "
               "or 'No'"));
                }
                RS_DIALOGFACTORY->requestOptions(this, true, true);
                setStatus(lastStatus);
            }
            break;*/

    default:
        break;
    }
}



RS_StringList RS_ActionModifyRound::getAvailableCommands() {
    RS_StringList cmd;
    switch (getStatus()) {
    case SetEntity1:
    case SetEntity2:
        cmd += command("radius");
        cmd += command("trim");
        break;
    default:
        break;
    }
    return cmd;
}



void RS_ActionModifyRound::showOptions() {
    RS_ActionInterface::showOptions();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, true);
    }
}



void RS_ActionModifyRound::hideOptions() {
    RS_ActionInterface::hideOptions();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, false);
    }
}



void RS_ActionModifyRound::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetEntity1:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify first entity"),
                                                tr("Back"));
            break;
        case SetEntity2:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify second entity"),
                                                tr("Back"));
            break;
        case SetRadius:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Enter radius:"),
                                                tr("Cancel"));
            break;
            /*case SetTrim:
                RS_DIALOGFACTORY->updateMouseWidget(tr("Trim on? (yes/no):"),
                                                    "");
                break;*/
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionModifyRound::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyRound::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
    }
}


// EOF
