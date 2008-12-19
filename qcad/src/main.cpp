/****************************************************************************
** $Id: main.cpp 2418 2005-06-17 11:01:04Z andrew $
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

#include "main.h"

#include <locale.h>

#include <qapplication.h>

#ifdef __APPLE__
#include <qmacstyle_mac.h>
#endif

#include <qtimer.h>

#if QT_VERSION>=0x030200
# include <qsplashscreen.h>
  QSplashScreen *splash = 0;
#endif

#ifdef RS_SCRIPTING
#include <qsproject.h>
#endif

#include "rs_fontlist.h"
#include "rs_patternlist.h"
#include "rs_scriptlist.h"
#include "rs_settings.h"
#include "rs_system.h"
#include "rs_fileio.h"
#include "rs_filtercxf.h"
#include "rs_filterdxf.h"
#include "rs_filterdxf1.h"

#include "qg_dlginitial.h"

#include "qc_applicationwindow.h"

#ifdef RS_CAM
# include "rs_filtercam.h"
# include "rs_camdialog.h"
#endif

#ifdef RS_CAM
# include "xpm/intro_camexpert.xpm"
# include "xpm/splash_camexpert.xpm"
#else
# include "xpm/intro_qcad.xpm"
# ifndef QC_CUSTOM_SPLASH
#  include "xpm/splash_qcad.xpm"
# endif
#endif

#ifndef QC_SPLASH_TXTCOL
# define QC_SPLASH_TXTCOL Qt::black
#endif

// for image mime resources from png files
extern void qInitImages_qcadactions();

#ifdef RS_SCRIPTING
	extern void qInitImages_qcadscripting();
#endif

#ifdef QC_BUILTIN_STYLE
	extern void applyBuiltinStyle();
#endif



/**
 * Main. Creates Application window.
 *
 * @todo Switch back to xpm instead of png. 
 * Cleaning up #defines.
 */
int main(int argc, char** argv) {
	
    RS_DEBUG->setLevel(RS_Debug::D_WARNING);
	RS_DEBUG->print("param 0: %s", argv[0]);

	// avoid . / , confusion on German environments
	setlocale(LC_ALL, "C");

    QApplication app(argc, argv);

	// for image mime resources from png files
	//  TODO: kinda dirty to call that explicitly
	qInitImages_qcadactions();
#ifdef RS_SCRIPTING
	qInitImages_qcadscripting();
#endif

	for (int i=0; i<app.argc(); i++) {
		if (QString("--debug") == app.argv()[i]) {
    		RS_DEBUG->setLevel(RS_Debug::D_DEBUGGING);
		}
	}
	
	QFileInfo prgInfo( QFile::decodeName(argv[0]) );
	QString prgDir(prgInfo.dirPath(true));
    RS_SETTINGS->init(QC_COMPANYKEY, QC_APPKEY);
    RS_SYSTEM->init(QC_APPNAME, QC_VERSION, QC_APPDIR, prgDir);
	
	RS_FILEIO->registerFilter(new RS_FilterCXF());
	RS_FILEIO->registerFilter(new RS_FilterDXF());
	RS_FILEIO->registerFilter(new RS_FilterDXF1());
#ifdef RS_OPT_CAM 
	RS_FILEIO->registerFilter(new RS_FilterCAM());
#endif

	// parse command line arguments that might not need a launched program:
	QStringList fileList = handleArgs(argc, argv);

	RS_String lang;
	RS_String langCmd;
	RS_String unit;
	
	RS_SETTINGS->beginGroup("/Defaults");
#ifndef QC_PREDEFINED_UNIT
	unit = RS_SETTINGS->readEntry("/Unit", "Invalid");
#else
	unit = RS_SETTINGS->readEntry("/Unit", QC_PREDEFINED_UNIT);
#endif
    RS_SETTINGS->endGroup();

	// show initial config dialog:
	if (unit=="Invalid") {
		RS_DEBUG->print("main: show initial config dialog..");
		QG_DlgInitial di(NULL);
		di.setText("<font size=\"+1\"><b>Welcome to " QC_APPNAME "</b></font>"
		"<br>"
		"Please choose the unit you want to use for new drawings and your "
		"preferred language.<br>"
		"You can changes these settings later in the "
		"Options Dialog of " QC_APPNAME ".");
		QPixmap pxm(intro_xpm);
		di.setPixmap(pxm);
		if (di.exec()) {
			RS_SETTINGS->beginGroup("/Defaults");
			unit = RS_SETTINGS->readEntry("/Unit", "None");
    		RS_SETTINGS->endGroup();
		}
		RS_DEBUG->print("main: show initial config dialog: OK");
	}

#ifdef QSPLASHSCREEN_H
	RS_DEBUG->print("main: splashscreen..");

# ifdef QC_CUSTOM_SPLASH
	QPixmap* pixmap = new QPixmap(QPixmap::fromMimeSource(QC_CUSTOM_SPLASH)); 
# else
	QPixmap* pixmap = new QPixmap(splash_xpm);
# endif

/*
	splash = new QSplashScreen(*pixmap);
	splash->show();
	splash->message(app.tr("Loading.."), 
		Qt::AlignRight|Qt::AlignBottom, QC_SPLASH_TXTCOL);
	RS_DEBUG->print("main: splashscreen: OK");
*/
#endif

	RS_DEBUG->print("main: init fontlist..");
    RS_FONTLIST->init();
	RS_DEBUG->print("main: init fontlist: OK");
	
	RS_DEBUG->print("main: init patternlist..");
    RS_PATTERNLIST->init();
	RS_DEBUG->print("main: init patternlist: OK");

	RS_DEBUG->print("main: init scriptlist..");
    RS_SCRIPTLIST->init();
	RS_DEBUG->print("main: init scriptlist: OK");

	RS_DEBUG->print("main: loading translation..");
	RS_SETTINGS->beginGroup("/Appearance");
#ifdef QC_PREDEFINED_LOCALE
		lang = RS_SETTINGS->readEntry("/Language", "");
		if (lang.isEmpty()) {
			lang=QC_PREDEFINED_LOCALE;
			RS_SETTINGS->writeEntry("/Language", lang);
		}
		langCmd = RS_SETTINGS->readEntry("/LanguageCmd", "");
		if (langCmd.isEmpty()) {
			langCmd=QC_PREDEFINED_LOCALE;
			RS_SETTINGS->writeEntry("/LanguageCmd", langCmd);
		}
#else
    lang = RS_SETTINGS->readEntry("/Language", "en");
    langCmd = RS_SETTINGS->readEntry("/LanguageCmd", "en");
#endif
	RS_SETTINGS->endGroup();
	
	RS_SYSTEM->loadTranslation(lang, langCmd);
	RS_DEBUG->print("main: loading translation: OK");

#ifdef QSPLASHSCREEN_H
	splash = new QSplashScreen(*pixmap);
	splash->show();
	splash->message(QObject::tr("Loading.."), 
		Qt::AlignRight|Qt::AlignBottom, QC_SPLASH_TXTCOL);
	RS_DEBUG->print("main: splashscreen: OK");
#endif

    //QApplication::setStyle(new QWindowsStyle());
    //QApplication::setStyle(new QPlatinumStyle());
#ifdef __APPLE__
	QApplication::setStyle(new QMacStyle());
#endif

#ifdef QC_BUILTIN_STYLE //js:
	RS_DEBUG->print("main: applying built in style..");
	applyBuiltinStyle();
#endif

	RS_DEBUG->print("main: creating main window..");
    QC_ApplicationWindow * appWin = new QC_ApplicationWindow();
	RS_DEBUG->print("main: setting caption");
    appWin->setCaption(QC_APPNAME);
	RS_DEBUG->print("main: show main window");
    appWin->show();
	RS_DEBUG->print("main: set focus");
	appWin->setFocus();
	RS_DEBUG->print("main: creating main window: OK");

#ifdef QSPLASHSCREEN_H
	if (splash) {
		RS_DEBUG->print("main: updating splash..");
		splash->message(QObject::tr("Loading Library Paths.."), 
			Qt::AlignRight|Qt::AlignBottom, QC_SPLASH_TXTCOL);
		RS_DEBUG->print("main: processing events");
		qApp->processEvents();
		RS_DEBUG->print("main: updating splash: OK");
	}
#endif

	// loading files:
	RS_DEBUG->print("main: loading files..");
	bool files_loaded = false;
	for (QStringList::Iterator it = fileList.begin(); it != fileList.end(); 
		++it ) {
		
#ifdef QSPLASHSCREEN_H
			if (splash) {
				splash->message(QObject::tr("Loading File %1..")
					.arg(QDir::convertSeparators(*it)), 
				Qt::AlignRight|Qt::AlignBottom, QC_SPLASH_TXTCOL);
				qApp->processEvents();
			}
#endif
			appWin->slotFileOpen(*it, RS2::FormatUnknown);
			files_loaded = true;
	}
	RS_DEBUG->print("main: loading files: OK");
	
#ifdef RS_DEMO
	QTimer::singleShot(10 * 60 * 1000, appWin, SLOT(slotFileDemoQuit()));
#endif
		
#ifdef QSPLASHSCREEN_H
# ifndef QC_DELAYED_SPLASH_SCREEN
	if (splash) {
		splash->finish(appWin);
		delete splash;
		splash = 0;
	}
# endif
	delete pixmap;
#endif

    //app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

	// renew: avoid . / , confusion on German environments
	//setlocale(LC_NUMERIC, "C");
	setlocale(LC_ALL, "C");
	
	RS_DEBUG->print("main: app.exec()");

	if (!files_loaded) {
		appWin->slotFileNew();
	}

	appWin->slotRunStartScript();
	
	int r = app.exec();

	delete appWin;

	RS_DEBUG->print("main: finished");

	return r;
}



/**
 * Handles command line arguments that might not require a GUI.
 *
 * @return list of files to load on startup.
 */
QStringList handleArgs(int argc, char** argv) {
	RS_DEBUG->print("main: handling args..");
	QStringList ret;

	bool doexit = false;
	QString machine;
	QString input;
	QString output;
	
	for (int i=1; i<argc; i++) {
		if (QString(argv[i]).startsWith("-")==false) {
			QString fname = QDir::convertSeparators( 
				QFileInfo(QFile::decodeName(argv[i])).absFilePath() );
			ret.append(fname);
		}
		else if (QString(argv[i])=="--exit") {
			doexit = true;
		}
#ifdef RS_CAM
		else if (QString(argv[i])=="--convert") {
			++i;
			if (i<argc) {
				machine = QString(argv[i]);
			}
			else {
        		RS_DEBUG->print(RS_Debug::D_WARNING,
					"No machine configuration given after --convert. " 
				    "Aborting..");
				exit(1);
			}
			++i;
			if (i<argc) {
				input = QString(argv[i]);
			}
			else {
        		RS_DEBUG->print(RS_Debug::D_WARNING,
					"No input given after --convert. " 
				    "Aborting..");
				exit(1);
			}
			++i;
			if (i<argc) {
				output = QString(argv[i]);
			}
			else {
        		RS_DEBUG->print(RS_Debug::D_WARNING,
					"No output given after --convert. " 
				    "Aborting..");
				exit(1);
			}
		}
#endif
	}

#ifdef RS_CAM
	// auto cam convert
	if (machine.isEmpty()==false && input.isEmpty()==false && 
		output.isEmpty()==false) {

		RS_FilterCAM fc;
		RS_Graphic gr;
		RS_FILEIO->fileImport(gr, input, RS2::FormatUnknown);
		RS_CamDialog dlg(gr, NULL);
		dlg.activateMachineGenerator(machine);
		dlg.externalOK();
		fc.sort(gr);
		fc.fileExport(gr, output, RS2::FormatCAM);
	}
#endif

	if (doexit) {
		exit(0);
	}
	
	RS_DEBUG->print("main: handling args: OK");
	return ret;
}

