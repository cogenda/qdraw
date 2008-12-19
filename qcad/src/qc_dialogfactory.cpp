/****************************************************************************
** $Id: qc_dialogfactory.cpp 897 2004-09-04 19:59:51Z andrew $
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

#include "qc_dialogfactory.h"

#include "qc_applicationwindow.h"



/**
 * Provides a new window for editing the active block.
 */
void QC_DialogFactory::requestEditBlockWindow(RS_BlockList* blockList) {
    RS_DEBUG->print("QC_DialogFactory::requestEditBlockWindow()");

    QC_ApplicationWindow* appWindow = QC_ApplicationWindow::getAppWindow();
    QC_MDIWindow* parent = appWindow->getMDIWindow();
    if (parent!=NULL) {
        //RS_BlockList* blist = blockWidget->getBlockList();
        if (blockList!=NULL) {
            RS_Block* blk = blockList->getActive();
            if (blk!=NULL) {
                QC_MDIWindow* w = appWindow->slotFileNew(blk);
                // the parent needs a pointer to the block window and
                //   vice versa
                parent->addChildWindow(w);
                w->getGraphicView()->zoomAuto(false);
            }
        }
    }
}



/**
 * Closes all windows that are editing the given block.
 */
void QC_DialogFactory::closeEditBlockWindow(RS_Block* block) {
	RS_DEBUG->print("QC_DialogFactory::closeEditBlockWindow");
	
	QC_ApplicationWindow* appWindow = QC_ApplicationWindow::getAppWindow();
	QWorkspace* workspace = appWindow->getWorkspace();

    if (workspace!=NULL) {
		RS_DEBUG->print("QC_DialogFactory::closeEditBlockWindow: workspace found");
		
        QWidgetList windows = workspace->windowList();
        for (int i = 0; i < int(windows.count()); ++i) {
			RS_DEBUG->print("QC_DialogFactory::closeEditBlockWindow: window: %d",
				i);
            QC_MDIWindow* m = (QC_MDIWindow*)windows.at(i);
            if (m!=NULL) {
				RS_DEBUG->print(
					"QC_DialogFactory::closeEditBlockWindow: got mdi");
				if (m->getDocument()==block) {
					RS_DEBUG->print(
						"QC_DialogFactory::closeEditBlockWindow: closing mdi");
					//m->closeMDI(true, false);
					m->close(true);
				}
			}
		}
	}
	appWindow->slotWindowActivated(NULL);
	
	RS_DEBUG->print("QC_DialogFactory::closeEditBlockWindow: OK");
}

