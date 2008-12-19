/****************************************************************************
** $Id: qc_mdiwindow.h 840 2004-03-18 21:13:31Z js $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the QCad project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid QCad Professional Edition licenses may use 
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

#ifndef QC_MDIWINDOW_H
#define QC_MDIWINDOW_H

#include <qmainwindow.h>

#if QT_VERSION>=0x030200
#include <qptrlist.h>
#else
#include <qlist.h>
#define QPtrList QList
#endif

#include "qc_graphicview.h"

#include "qg_layerwidget.h"
#include "qg_recentfiles.h"
#include "qg_pentoolbar.h"

#include "rs_document.h"


/**
 * MDI document window. Contains a document and a view (window).
 *
 * @author Andrew Mustun
 */
class QC_MDIWindow: public QMainWindow {
    Q_OBJECT

public:
    QC_MDIWindow(RS_Document* doc,
                 QWidget* parent,
                 const char* name=NULL,
                 int wflags=WDestructiveClose);
    ~QC_MDIWindow();

    void initDoc(RS_Document* doc=NULL);
    void initView();

public slots:

    void slotPenChanged(RS_Pen p);

    void slotFileNew();
    bool slotFileOpen(const QString& fileName, RS2::FormatType type);
    bool slotFileSave(bool &cancelled);
    bool slotFileSaveAs(bool &cancelled);
    bool slotFileClose(bool force);
    void slotFilePrint();

public:
    /** @return Pointer to graphic view */
    QC_GraphicView* getGraphicView() {
        return graphicView;
    }

    /** @return Pointer to document */
    RS_Document* getDocument() {
        return document;
    }
	
    /** @return Pointer to graphic or NULL */
    RS_Graphic* getGraphic() {
        return document->getGraphic();
    }

	/** @return Pointer to current event handler */
	RS_EventHandler* getEventHandler() {
		if (graphicView!=NULL) {
			return graphicView->getEventHandler();
		}
		else {
			return NULL;
		}
	}

    void addChildWindow(QC_MDIWindow* w);
    void removeChildWindow(QC_MDIWindow* w);
    QC_MDIWindow* getPrintPreview();

    /**
     * Sets the parent window that will be notified if this 
     */
    void setParentWindow(QC_MDIWindow* p) {
        RS_DEBUG->print("setParentWindow");
        parentWindow = p;
    }

    /**
     * @return The MDI window id.
     */
    int getId() {
        return id;
    }

	bool closeMDI(bool force, bool ask=true);

	void setForceClosing(bool on) {
		forceClosing = on;
	}

    friend std::ostream& operator << (std::ostream& os, QC_MDIWindow& w);

signals:
    void signalClosing();

protected:
    void closeEvent(QCloseEvent*);

private:
    /** window ID */
    int id;
    /** ID counter */
    static int idCounter;
    /** Graphic view */
    QC_GraphicView* graphicView;
    /** Document */
    RS_Document* document;
    /** Does the window own the document? */
    bool owner;
    /**
     * List of known child windows that show blocks of the same drawing.
     */
    QPtrList<QC_MDIWindow> childWindows;
    /**
     * Pointer to parent window which needs to know if this window 
     * is closed or NULL.
     */
    QC_MDIWindow* parentWindow;

	/**
	 * If flag is set, the user will not be asked about closing this file.
	 */
	bool forceClosing;
};


#endif

