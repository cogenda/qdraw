/****************************************************************************
** $Id: rs_actionlibraryinsert.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONLIBRARYINSERT_H
#define RS_ACTIONLIBRARYINSERT_H

#include "rs_previewactioninterface.h"

#include "rs_creation.h"
#include "rs_insert.h"

/**
 * This action class can handle user events for inserting library items 
 * (or any other DXF files) into the current drawing (as block/insert).
 *
 * @author Andrew Mustun
 */
class RS_ActionLibraryInsert : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetTargetPoint,    /**< Setting the reference point. */
		SetAngle,          /**< Setting angle in the command line. */
		SetFactor          /**< Setting factor in the command line. */
		//SetColumns,        /**< Setting columns in the command line. */
		//SetRows,           /**< Setting rows in the command line. */
		//SetColumnSpacing,  /**< Setting column spacing in the command line. */
		//SetRowSpacing      /**< Setting row spacing in the command line. */
    };

public:
    RS_ActionLibraryInsert(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    virtual ~RS_ActionLibraryInsert();

	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);
	
	virtual RS2::ActionType rtti() {
		return RS2::ActionLibraryInsert;
	}

    virtual void init(int status=0);

	void reset();

    virtual void trigger();

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

    virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();

	virtual void showOptions();
	virtual void hideOptions();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

	void setFile(const RS_String& file);

	double getAngle() {
		return data.angle;
	}

	void setAngle(double a) {
		data.angle = a;
	}

	double getFactor() {
		return data.factor;
	}

	void setFactor(double f) {
		data.factor = f;
	}

	/*int getColumns() {
		return data.cols;
	}

	void setColumns(int c) {
		data.cols = c;
	}
	
	int getRows() {
		return data.rows;
	}

	void setRows(int r) {
		data.rows = r;
	}

	double getColumnSpacing() {
		return data.spacing.x;
	}

	void setColumnSpacing(double cs) {
		data.spacing.x = cs;
	}
	
	double getRowSpacing() {
		return data.spacing.y;
	}

	void setRowSpacing(double rs) {
		data.spacing.y = rs;
	}*/

protected:
	//RS_Block* block;
	//RS_InsertData data;
	RS_Graphic prev;
	RS_LibraryInsertData data;
	
	/** Last status before entering option. */
	Status lastStatus;
};

#endif
