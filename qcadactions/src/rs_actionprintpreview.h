/****************************************************************************
** $Id: rs_actionprintpreview.h 1117 2004-04-09 19:57:47Z andrew $
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

#ifndef RS_ACTIONPRINTPREVIEW_H
#define RS_ACTIONPRINTPREVIEW_H

#include "rs_actioninterface.h"

/**
 * Default action for print preview.
 *
 * @author Andrew Mustun
 */
class RS_ActionPrintPreview : public RS_ActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
		Neutral,
		Moving
    };

public:
    RS_ActionPrintPreview(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionPrintPreview();

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

	virtual RS2::ActionType rtti() {
		return RS2::ActionPrintPreview;
	}

    virtual void init(int status=0);

    virtual void trigger();

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mousePressEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

    virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();

	virtual void showOptions();
	virtual void hideOptions();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

	void center();
	void fit();
	void setScale(double f);
	double getScale();

	void setBlackWhite(bool bw);
	//bool isBlackWhite() {
	//	return blackWhite;
	//}
	RS2::Unit getUnit();

protected:
	//bool blackWhite;
	RS_Vector v1;
	RS_Vector v2;
};

#endif
