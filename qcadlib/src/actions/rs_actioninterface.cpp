/****************************************************************************
** $Id: rs_actioninterface.cpp 1938 2004-12-09 23:09:53Z andrew $
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


#include "rs_actioninterface.h"

/**
 * Constructor.
 *
 * Sets the entity container on which the action class inherited 
 * from this interface operates.
 *
 * @param name Action name. This can be used internally for
 *             debugging mainly.
 * @param container Entity container this action operates on.
 * @param graphicView Graphic view instance this action operates on.
 *                    Please note that an action belongs to this
 *                    view.
 * @param cursor Default mouse cursor for this action. If the action
 *               is suspended and resumed again the cursor will always
 *               be reset to the one given here.
 */
RS_ActionInterface::RS_ActionInterface(const char* name,
                                       RS_EntityContainer& container,
                                       RS_GraphicView& graphicView) :
RS_Snapper(container, graphicView) {

    RS_DEBUG->print("RS_ActionInterface::RS_ActionInterface: Setting up action: \"%s\"", name);
	
    this->name = name;
    status = 0;
    finished = false;
    //triggerOnResume = false;

    // graphic provides a pointer to the graphic if the
    // entity container is a graphic (i.e. can also hold
    // layers).
    graphic = container.getGraphic();

    // document pointer will be used for undo / redo
    document = container.getDocument();

    //this->cursor = cursor;
    //setSnapMode(graphicView.getDefaultSnapMode());


    RS_DEBUG->print("RS_ActionInterface::RS_ActionInterface: Setting up action: \"%s\": OK", name);
}

/**
 * Destructor.
 */
RS_ActionInterface::~RS_ActionInterface() {
    // would be pure virtual now:
    // hideOptions();
}

/**
 * Must be implemented to return the ID of this action.
*
* @todo no default implementation
 */
RS2::ActionType RS_ActionInterface::rtti()  {
    return RS2::ActionNone;
}

/**
 * @return name of this action
 */
RS_String RS_ActionInterface::getName() {
    return name;
}

/**
 * Called to initiate an action. This funtcion is often
 * overwritten by the implementing action.
 *
 * @param status The status on which to initiate this action.
 * default is 0 to begin the action.
 */
void RS_ActionInterface::init(int status) {
    RS_Snapper::init();

    setStatus(status);
    if (status>=0) {
        //graphicView->setMouseCursor(cursor);
		updateMouseButtonHints();
        updateMouseCursor();
        updateToolBar();
    }
}



/**
 * Called when the mouse moves and this is the current action. 
 * This function can be overwritten by the implementing action.
 * The default implementation keeps track of the mouse position.
 */
void RS_ActionInterface::mouseMoveEvent(RS_MouseEvent*) {}

/**
 * Called when the left mouse button is pressed and this is the 
 * current action. 
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::mousePressEvent(RS_MouseEvent*) {}

/**
 * Called when the left mouse button is released and this is 
 * the current action. 
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::mouseReleaseEvent(RS_MouseEvent*) {}

/**
 * Called when a key is pressed and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::keyPressEvent(RS_KeyEvent* e) {
    e->ignore();
}

/**
 * Called when a key is released and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::keyReleaseEvent(RS_KeyEvent* e) {
    e->ignore();
}

/**
 * Coordinate event. Triggered usually from a command line.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::coordinateEvent(RS_CoordinateEvent*) {}

/**
 * Called when a command from the command line is launched.
 * and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::commandEvent(RS_CommandEvent*) {
}

/**
 * Must be implemented to return the currently available commands
 *  for the command line.
 */
RS_StringList RS_ActionInterface::getAvailableCommands() {
    RS_StringList l;
    return l;
}

/**
 * Sets the current status (progress) of this action.
 * The default implementation sets the class variable 'status' to the
 * given value and finishes the action if 'status' is negative.
 *
 * @param status Status number. It's up to the action implementor
 *               what the action uses the status for. However, a
 *               negative status number finishes the action. Usually
 *               the status of an action increases for every step
 *               of progress and decreases when the user goes one
 *               step back (i.e. presses the right mouse button).
 */
void RS_ActionInterface::setStatus(int status) {
    this->status = status;
    if (status<0) {
        finish();
        status=0;
    }
    updateMouseButtonHints();
    updateToolBar();
    updateMouseCursor();
}

/**
 * @return Current status of this action.
 */
int RS_ActionInterface::getStatus() {
    return status;
}

/**
 * Triggers this action. This should be called after all
 * data needed for this action was collected / set.
 * The default implementation does nothing.
 */
void RS_ActionInterface::trigger() {}

/**
 * Should be overwritten to update the mouse button hints
 * wherever they might needed.
 */
void RS_ActionInterface::updateMouseButtonHints() {}

/**
 * Should be overwritten to set the mouse cursor for this action.
 */
void RS_ActionInterface::updateMouseCursor() {}

/**
 * Should be overwritten to set the toolbar for this action.
 */
void RS_ActionInterface::updateToolBar() {}

/**
 * @return true, if the action is finished and can be deleted.
 */
bool RS_ActionInterface::isFinished() {
    return finished;
}


/**
 * Forces a termination of the action without any cleanup.
 */
void RS_ActionInterface::setFinished() {
	status = -1;
}


/**
 * Finishes this action.
 */
void RS_ActionInterface::finish() {
	RS_DEBUG->print("RS_ActionInterface::finish");
    status = -1;
    graphicView->setMouseCursor(RS2::ArrowCursor);
    //graphicView->requestToolBar(RS2::ToolBarMain);
    updateToolBar();
    deleteSnapper();
    hideOptions();
    finished = true;
    RS_Snapper::finish();
	RS_DEBUG->print("RS_ActionInterface::finish: OK");
}

/**
 * Called by the event handler to give this action a chance to
 * communicate with its predecessor.
 */
void RS_ActionInterface::setPredecessor(RS_ActionInterface* pre) {
    predecessor = pre;
}

/**
 * Suspends this action while another action takes place.
 */
void RS_ActionInterface::suspend() {
    graphicView->setMouseCursor(RS2::ArrowCursor);
    RS_Snapper::suspend();
}

/**
 * Resumes an action after it was suspended.
 */
void RS_ActionInterface::resume() {
    updateMouseCursor();
    updateToolBar();
    RS_Snapper::resume();
}

/**
 * Hides the tool options. Default implementation does nothing.
 */
void RS_ActionInterface::hideOptions() {
    RS_Snapper::hideOptions();
}

/**
 * Shows the tool options. Default implementation does nothing.
 */
void RS_ActionInterface::showOptions() {
    RS_Snapper::showOptions();
}

/**
 * Calls checkCommand() from the RS_COMMANDS module.
 */
bool RS_ActionInterface::checkCommand(const RS_String& cmd, const RS_String& str,
                                      RS2::ActionType action) {
#ifndef RS_NO_QCADCMD
    return RS_COMMANDS->checkCommand(cmd, str, action);
#else
    return false;
#endif
}

/**
 * Calls command() from the RS_COMMANDS module.
 */
RS_String RS_ActionInterface::command(const RS_String& cmd) {
#ifndef RS_NO_QCADCMD
    return RS_COMMANDS->command(cmd);
#else 
	return cmd;
#endif
}

/**
 * Calls msgAvailableCommands() from the RS_COMMANDS module.
 */
RS_String RS_ActionInterface::msgAvailableCommands() {
#ifndef RS_NO_QCADCMD
    return RS_COMMANDS->msgAvailableCommands();
#else
	return "";
#endif
}

