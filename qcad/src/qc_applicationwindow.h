/****************************************************************************
** $Id: qc_applicationwindow.h 934 2005-02-16 22:22:56Z andrew $
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

#ifndef QC_APPLICATIONWINDOW_H
#define QC_APPLICATIONWINDOW_H


#include <qaction.h>
#include <qassistantclient.h>
#include <qlayout.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qsplitter.h>
#include <qstatusbar.h>
#include <qtable.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>

#undef QT_NO_WORKSPACE
#include <qworkspace.h>

#include "rs_document.h"

#include "qc_dialogfactory.h"
#include "qc_graphicview.h"
#include "qc_mdiwindow.h"

#include "qg_actionhandler.h"
#include "qg_actionfactory.h"
#include "qg_blockwidget.h"
#include "qg_layerwidget.h"
#include "qg_mainwindowinterface.h"
#include "qg_recentfiles.h"
#include "qg_pentoolbar.h"

#ifdef RS_SCRIPTING
#include "qs_scripter.h"
#include <qsproject.h>
#endif

#if QT_VERSION>=0x030000
#include "qg_cadtoolbar.h"
#include "qg_commandwidget.h"
#include "qg_librarywidget.h"
#else
class QG_CadToolBar;
class QG_CommandWidget;
class QG_LibraryWidget;
#endif

#ifdef RS_CAM
#include "rs_simulationcontrols.h"
#endif



/**
 * Main application window. Hold together document, view and controls.
 *
 * @author Andrew Mustun
 */
class QC_ApplicationWindow: public QMainWindow,
    public QG_MainWindowInterface
	{
    Q_OBJECT

public:
    QC_ApplicationWindow();
    ~QC_ApplicationWindow();

    void initActions();
    void initMenuBar();
    void initToolBar();
    void initStatusBar();

    void initSettings();
	void restoreDocks();
    void storeSettings();

    void updateRecentFilesMenu();

    void initMDI();
    void initView();

    bool queryExit(bool force);

	/** Catch hotkey for giving focus to command line. */
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);

public slots:
    virtual void show();
    void finishSplashScreen();
	void slotFocus();
    void slotBack();
    //void slotNext();
    void slotEnter();
    void slotFocusCommandLine();
	void slotError(const QString& msg);

    void slotWindowActivated(QWidget* w);
    void slotWindowsMenuAboutToShow();
    void slotWindowsMenuActivated(int);
    void slotTileHorizontal();
    void slotTileVertical();

    void slotPenChanged(RS_Pen p);

    /** generates a new document for a graphic. */
    QC_MDIWindow* slotFileNew(RS_Document* doc=NULL);
    /** opens a document */
    void slotFileOpen();
    /**
    * opens a recent file document
    * @param id File Menu id of the file
    */
    void slotFileOpenRecent(int id);
    /**
     * opens the given file.
     */
    void slotFileOpen(const QString& fileName, RS2::FormatType type);
    /** saves a document */
    void slotFileSave();
    /** saves a document under a different filename*/
    void slotFileSaveAs();
	/** exports the document as bitmap */
	void slotFileExport();
	bool slotFileExport(const QString& name, const QString& format, 
		QSize size, bool black, bool bw=false);
    /** closes the current file */
    void slotFileClose();
    /** closing the current file */
    void slotFileClosing();
    /** prints the current file */
    void slotFilePrint();
    /** shows print preview of the current file */
    void slotFilePrintPreview(bool on);
    /** exits the application */
    void slotFileQuit();

	/** forces to quit QCad (demo) */
	void slotFileDemoQuit();

    /** toggle the grid */
    void slotViewGrid(bool toggle);
    /** toggle the draft mode */
    void slotViewDraft(bool toggle);
    /** toggle the statusbar */
    void slotViewStatusBar(bool toggle);
    /** toggle the layerlist */
    //void slotViewLayerList(bool toggle);
    /** toggle the blocklist */
    //void slotViewBlockList(bool toggle);
    /** toggle the command line */
    //void slotViewCommandLine(bool toggle);
    /** toggle the option toolbar */
    //void slotViewOptionToolbar(bool toggle);

    //void slotBlocksEdit();
    void slotOptionsGeneral();
	
    void slotScriptOpenIDE();
    void slotScriptRun();
	
	void slotRunStartScript();
	void slotRunScript();
	void slotRunScript(const QString& name);
	
	void slotInsertBlock();
	void slotInsertBlock(const QString& name);

    /** shows an about dlg*/
    void slotHelpAbout();
    void slotHelpManual();

    /** dumps entities to file */
    void slotTestDumpEntities(RS_EntityContainer* d=NULL);
    /** dumps undo info to stdout */
    void slotTestDumpUndo();
    /** updates all inserts */
    void slotTestUpdateInserts();
    /** draws some random lines */
    void slotTestDrawFreehand();
    /** inserts a test block */
    void slotTestInsertBlock();
    /** inserts a test ellipse */
    void slotTestInsertEllipse();
    /** inserts a test text */
    void slotTestInsertText();
    /** inserts a test image */
    void slotTestInsertImage();
    /** unicode table */
    void slotTestUnicode();
    /** math experimental */
    void slotTestMath01();
    /** resizes window to 640x480 for screen shots */
    void slotTestResize640();
    /** resizes window to 640x480 for screen shots */
    void slotTestResize800();
    /** resizes window to 640x480 for screen shots */
    void slotTestResize1024();

signals:
    void gridChanged(bool on);
    void draftChanged(bool on);
    void printPreviewChanged(bool on);
    void windowsChanged(bool windowsLeft);

public:
    /**
     * @return Pointer to application window.
     */
    static QC_ApplicationWindow* getAppWindow() {
        return appWindow;
    }

    /**
     * @return Pointer to workspace.
     */
    QWorkspace* getWorkspace() {
        return workspace;
    }

    /**
     * @return Pointer to the currently active MDI Window or NULL if no
     * MDI Window is active.
     */
    QC_MDIWindow* getMDIWindow() {
		if (workspace!=NULL) {
        	return (QC_MDIWindow*)workspace->activeWindow();
		}
		else {
			return NULL;
		}
    }

    /**
     * Implementation from RS_MainWindowInterface (and QS_ScripterHostInterface).
     *
     * @return Pointer to the graphic view of the currently active document
     * window or NULL if no window is available.
     */
    virtual RS_GraphicView* getGraphicView() {
        QC_MDIWindow* m = getMDIWindow();
        if (m!=NULL) {
            return m->getGraphicView();
        }
        return NULL;
    }

    /**
     * Implementation from RS_MainWindowInterface (and QS_ScripterHostInterface).
     *
     * @return Pointer to the graphic document of the currently active document
     * window or NULL if no window is available.
     */
    virtual RS_Document* getDocument() {
        QC_MDIWindow* m = getMDIWindow();
        if (m!=NULL) {
            return m->getDocument();
        }
        return NULL;
    }
	
	/**
	 * Creates a new document. Implementation from RS_MainWindowInterface.
	 */
	virtual void createNewDocument(
		const RS_String& fileName = RS_String::null, RS_Document* doc=NULL) {
		
		slotFileNew(doc);
		if (fileName!=RS_String::null && getDocument()!=NULL) {
			getDocument()->setFilename(fileName);
		}
	}

    /**
     * Implementation from QG_MainWindowInterface.
     *
     * @return Pointer to this.
     */
    virtual QMainWindow* getMainWindow() {
        return this;
    }

    /**
     * @return Pointer to action handler. Implementation from QG_MainWindowInterface.
     */
    virtual QG_ActionHandler* getActionHandler() {
        return actionHandler;
    }

	/**
	 * Implementation from QG_MainWindowInterface.
	 */
	virtual void showSimulationControls() {
#ifdef RS_CAM
		simulationDockWindow->show();
#endif
	}

	//virtual QToolBar* createToolBar(const RS_String& name);
	//virtual void addToolBarButton(QToolBar* tb);
	
    /**
     * @return Pointer to the qsa object.
     */
#ifdef RS_SCRIPTING
    QSProject* getQSAProject() {
		if (scripter!=NULL) {
	        return scripter->getQSAProject();
		}
		else {
			return NULL;
		}
    }
#endif

	void redrawAll();
	void updateGrids();

	/**
	 * Implementation from QG_MainWindowInterface.
	 */
	virtual void setFocus2() {
		setFocus();
	}

protected:
    void closeEvent(QCloseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent* e);

private:
    /** Pointer to the application window (this). */
    static QC_ApplicationWindow* appWindow;

    /** Workspace for MDI */
    QWorkspace* workspace;

	/** Dialog factory */
	QC_DialogFactory* dialogFactory;

    /** Layer list widget */
    QG_LayerWidget* layerWidget;
    /** Block list widget */
    QG_BlockWidget* blockWidget;
    /** Library browser widget */
    QG_LibraryWidget* libraryWidget;
#ifdef RS_CAM
	/** CAM Simulation panel */
	RS_SimulationControls* simulationControls;
#endif

#if QT_VERSION>=0x030000
    /** Layer list dock widget */
    QDockWindow* layerDockWindow;
    /** Block list dock widget */
    QDockWindow* blockDockWindow;
    /** Library list dock widget */
    QDockWindow* libraryDockWindow;
#ifdef RS_CAM
    /** Simulation controls dock widget */
    QDockWindow* simulationDockWindow;
#endif
#endif

	/** Command line */
	QG_CommandWidget* commandWidget;
#if QT_VERSION>=0x030000
	QDockWindow* commandDockWindow;
#endif

    /** Coordinate widget */
    QG_CoordinateWidget* coordinateWidget;
    /** Mouse widget */
    QG_MouseWidget* mouseWidget;
    /** Selection Status */
    QG_SelectionWidget* selectionWidget;

    /** Option widget for individual tool options */
    QToolBar* optionWidget;

    /** Recent files list */
    QG_RecentFiles* recentFiles;

    /** Action handler. */
	QG_ActionHandler* actionHandler;

#ifdef RS_SCRIPTING
	/** Scripting interface. */
	QS_Scripter* scripter;
#endif

    QPopupMenu* fileMenu;
    QPopupMenu* windowsMenu;
    QPopupMenu* scriptMenu;
    QPopupMenu* helpMenu;
    QPopupMenu* testMenu;

    /** the main toolbars */
    QToolBar* fileToolBar;
    QToolBar* editToolBar;
    QToolBar* zoomToolBar;

    // Toolbar for selecting the current pen
    QG_PenToolBar* penToolBar;
    // Toolbar for CAD tools
    QG_CadToolBar* cadToolBar;

	QAssistantClient* assistant;

    QAction* scriptOpenIDE;
    QAction* scriptRun;
	
    QAction* helpAboutApp;
    QAction* helpManual;

    QAction *testDumpEntities;
    QAction *testDumpUndo;
    QAction *testUpdateInserts;
    QAction *testDrawFreehand;
    QAction *testInsertBlock;
    QAction *testInsertText;
    QAction *testInsertImage;
    QAction *testUnicode;
    QAction *testInsertEllipse;

    QAction *testMath01;

    QAction *testResize640;
    QAction *testResize800;
    QAction *testResize1024;

};


#endif

