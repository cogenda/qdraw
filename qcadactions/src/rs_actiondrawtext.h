/****************************************************************************
** $Id: rs_actiondrawtext.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONDRAWTEXT_H
#define RS_ACTIONDRAWTEXT_H

#include "rs_previewactioninterface.h"
#include "rs_text.h"


/**
 * This action class can handle user events to draw texts.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawText : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        ShowDialog,           /**< Showing the text dialog. */
        SetPos,               /**< Setting the position. */
		SetText               /**< Settting the text in the command line. */
    };

public:
    RS_ActionDrawText(RS_EntityContainer& container,
                      RS_GraphicView& graphicView);
    ~RS_ActionDrawText() {}

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);
	
	virtual RS2::ActionType rtti() {
		return RS2::ActionDrawText;
	}

    virtual void init(int status=0);

	void reset();
	
    virtual void trigger();
	void preparePreview();
	
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
	
	virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();

    virtual void hideOptions();
    virtual void showOptions();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

	void setText(const RS_String& t);
	RS_String getText();

	void setAngle(double a);
	double getAngle();

private:
    RS_TextData data;
	//RS_Text* text;
    RS_Vector pos;
	bool textChanged;
};

#endif
