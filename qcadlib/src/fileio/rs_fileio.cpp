/****************************************************************************
** $Id: rs_fileio.cpp 1938 2004-12-09 23:09:53Z andrew $
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

#include "rs_fileio.h"
#include "rs_filtercxf.h"
#include "rs_filterdxf.h"
#include "rs_filterdxf1.h"
#include "rs_fileinfo.h"
#include "rs_textstream.h"


RS_FileIO* RS_FileIO::uniqueInstance = NULL;

/**
 * Calls the import method of the filter responsible for the format
 * of the given file.
 *
 * @param graphic The container to which we will add
 *        entities. Usually that's an RS_Graphic entity but
 *        it can also be a polyline, text, ...
 * @param file Path and name of the file to import.
 */
bool RS_FileIO::fileImport(RS_Graphic& graphic, const RS_String& file, 
		RS2::FormatType type) {

    RS_DEBUG->print("Trying to import file '%s'...", file.latin1());

    RS_FilterInterface* filter = NULL;

	RS2::FormatType t;
	if (type == RS2::FormatUnknown) {
		t = detectFormat(file);
	}
	else {
		t = type;
	}

	filter = getImportFilter(t);

	/*
	switch (t) {
	case RS2::FormatCXF:
        filter = new RS_FilterCXF(graphic);
		break;

	case RS2::FormatDXF1:
        filter = new RS_FilterDXF1(graphic);
		break;

	case RS2::FormatDXF:
        filter = new RS_FilterDXF(graphic);
		break;

	default:
		break;
    }
	*/

    if (filter!=NULL) {
        return filter->fileImport(graphic, file, t);
    }
	else {
		RS_DEBUG->print(RS_Debug::D_WARNING,
			"RS_FileIO::fileImport: failed to import file: %s", 
			file.latin1());
	}
	
	return false;
}



/**
 * Calls the export method of the object responsible for the format
 * of the given file.
 *
 * @param file Path and name of the file to import.
 */
bool RS_FileIO::fileExport(RS_Graphic& graphic, const RS_String& file,
		RS2::FormatType type) {

    RS_DEBUG->print("RS_FileIO::fileExport");
    //RS_DEBUG->print("Trying to export file '%s'...", file.latin1());

	if (type==RS2::FormatUnknown) {
    	RS_String extension;
    	extension = RS_FileInfo(file).extension(false).lower();

		if (extension=="dxf") {
			type = RS2::FormatDXF;
		}
		else if (extension=="cxf") {
			type = RS2::FormatCXF;
		}
	}

	RS_FilterInterface* filter = getExportFilter(type);
	if (filter!=NULL) {
		return filter->fileExport(graphic, file, type);
	}
	
    RS_DEBUG->print("RS_FileIO::fileExport: no filter found");

	return false;
}


/**
 * Detects and returns the file format of the given file.
 */
RS2::FormatType RS_FileIO::detectFormat(const RS_String& file) {
    RS2::FormatType type = RS2::FormatUnknown;
    RS_FileInfo f(file);

    RS_String ext = f.extension(false).lower();
    if (ext=="cxf") {
        type = RS2::FormatCXF;
    } else if (ext=="dxf") {
        type = RS2::FormatDXF1;
        RS_File f(file);

        if (!f.open(IO_ReadOnly)) {
            // Error opening file:
            RS_DEBUG->print(RS_Debug::D_WARNING,
				"RS_FileIO::detectFormat: Cannot open file: %s", file.latin1());
            type = RS2::FormatUnknown;
        } else {
            RS_DEBUG->print("RS_FileIO::detectFormat: "
				"Successfully opened DXF file: %s",
                file.latin1());

            RS_TextStream ts(&f);
            RS_String line;
            int c=0;
            while (!f.atEnd() && ++c<100) {
                line = ts.readLine();
                if (line=="$ACADVER") {
                    type = RS2::FormatDXF;
                }
				// very simple reduced DXF:
                if (line=="ENTITIES" && c<10) {
					type = RS2::FormatDXF;
				}
            }
            f.close();
        }
    }

    return type;
}


// EOF
