/****************************************************************************
** $Id: rs_actionsetsnaprestriction.cpp 945 2003-06-11 06:56:54Z andrew $
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

#include "rs_actionsetsnaprestriction.h"



/**
 * Constructor.
 *
 * @param snapRes The new snap mode used from now on.
 * @param set true: set additional snap mode / false: unset
 */
RS_ActionSetSnapRestriction::RS_ActionSetSnapRestriction(
        RS_EntityContainer& container,
        RS_GraphicView& graphicView,
        RS2::SnapRestriction snapRes)
        :RS_ActionInterface("Set Additional Snap Mode", container, graphicView) {

    this->snapRes = snapRes;
}



void RS_ActionSetSnapRestriction::trigger() {
    graphicView->setSnapRestriction(snapRes);
	
    finish();
}



void RS_ActionSetSnapRestriction::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
