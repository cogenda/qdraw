/****************************************************************************
** $Id: test.cpp 2363 2005-04-04 14:56:55Z andrew $
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

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "rs_block.h"
#include "rs_entity.h"
#include "rs_entitycontainer.h"
#include "rs_creation.h"
#include "rs_graphic.h"
#include "rs_debug.h"
#include "rs_information.h"
#include "rs_insert.h"
#include "rs_fileio.h"
#include "rs_vector.h"
#include "rs_fontlist.h"
#include "rs_system.h"


void testVector();
void testUndo();
void testFontList();
void testSystem();

void testPoint();
void testLine();
void testRectangle();
void testPolyline();
void testHierarchy();
void testBlocks();
void testCloning();

void testDistanceLine();
void testNearestPointOnEntity();
void testIntersection();

void testDXFImport();
void testDXFExport();

void testMath();

void usage();


/**
 * Launches different test routines for the core of QCad.
 */

int main(int argc, char** argv) {

    // Check given arguments:
    if (argc<1) {
        usage();
    } else {
        RS_DEBUG->setLevel(RS_Debug::D_DEBUGGING);

        //testVector();
        //testUndo();
        //testFontList();
        //testSystem();

        //RS_DEBUG->setLevel(RS_Debug::NOTHING);

        //testPoint();
        //testLine();
        //testRectangle();
        //testPolyline();
        //testHierarchy();
        //testDistanceLine();

        //testDXFExport();
        //testDXFImport();

        //testMath();

        //testBlocks();

        testCloning();

        //testNearestPointOnEntity();
        //testIntersection();

        //testUndo();


        /*
        RS_DEBUG->print (RS_Debug::D_DEBUGGING, "Getting information from the graphic...");

        double dist = 0.0;
        RS_Vector ep = info.getNearestEndPoint(RS_Vector(30.0, 3.0), &dist);
        std::cout << "Nearest end point: " << ep << "\n";
        std::cout << "Distance: " << dist << "\n";
        */

    }

    RS_Debug::deleteInstance();

}



/**
 * Testing RS_Vector.
 */
void testVector() {
    //RS_Vector::test();
}



/**
 * Testing RS_Undo.
 */
void testUndo() {
    RS_Undo::test();

    /*
       RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting Undo");

    RS_DEBUG->print("Creating graphic");
       RS_Graphic* graphic = new RS_Graphic();

    RS_DEBUG->print("  adding line");
       RS_Line* line = new RS_Line(graphic,
                                   RS_Vector(10.0,10.0),
                                   RS_Vector(5.0, 50.0));
    graphic->addEntity(line);

    RS_DEBUG->print("  adding undoable");
    RS_UndoCycle* u = new RS_UndoCycle();
    u->addUndoable(line);
    graphic->addUndoCycle(u);

    std::cout << *graphic << "\n\n";

    graphic->undo();

    std::cout << ">> UNDO <<\n";

    std::cout << *graphic << "\n\n";
    */
}



/**
 * Testing RS_FontList.
 */
void testFontList() {
    //RS_FontList::test();
}



/**
 * Testing RS_System.
 */
void testSystem() {
    //RS_System::test();
}



/**
 * Test routine for points.
 */
void testPoint() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting point entities");

    RS_Graphic graphic;
    //RS_Creation creation(graphic);

    //creation.createPoint(RS_Vector(10.0, 20.0));
    graphic.addEntity(
        new RS_Point(&graphic, RS_PointData(RS_Vector(10.0, 20.0)))
    );

    std::cout << graphic;

    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "Testing point entities done\n\n");
}


/**
 * Test routine for lines.
 */
void testLine() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting line entities");

    RS_Graphic graphic;
    //RS_Creation creation(graphic);

    //creation.createLine2P(RS_Vector(10.0, 20.0), RS_Vector(11.0, 21.0));
    graphic.addEntity(
        new RS_Line(&graphic, RS_LineData(RS_Vector(10.0, 20.0),
                                          RS_Vector(11.0, 21.0)))
    );

    std::cout << graphic;

    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "Testing line entities done\n\n");
}


/**
 * Test routine for rectangles.
 */
void testRectangle() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting rectangle entities");

    //RS_Graphic graphic;
    //RS_Creation creation(graphic);

    //creation.createRectangle(RS_Vector(5.0, 2.0), RS_Vector(7.5, 3.0));

    //std::cout << graphic;

    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "Testing rectangle entities done\n\n");
}


/**
 * Test routine for polylines.
 */
void testPolyline() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting polyline entities");

    RS_Graphic graphic;
    //RS_Creation creation(graphic);

    RS_Polyline *pl = new RS_Polyline(&graphic,
                                      RS_PolylineData());
    pl->addVertex(RS_Vector(1.0, 5.0), 0.0);
    pl->addVertex(RS_Vector(10.0, 10.0), 0.0);
    pl->addVertex(RS_Vector(50.0, 15.0), 0.0);
    pl->addVertex(RS_Vector(100.0, 0.0), 0.0);

    graphic.addEntity(pl);

    std::cout << graphic;

    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "Testing polyline entities done\n\n");
}


/**
 * Test routine for hierarchies.
 */
void testHierarchy() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting hierarchical entities");

    RS_Graphic graphic;
    RS_Graphic* group1 = new RS_Graphic();
    group1->addEntity(new RS_Line(group1,
                                  RS_LineData(RS_Vector(1,1), RS_Vector(1,1))));
    group1->addEntity(new RS_Line(group1,
                                  RS_LineData(RS_Vector(1,2), RS_Vector(1,2))));
    group1->addEntity(new RS_Line(group1,
                                  RS_LineData(RS_Vector(1,3), RS_Vector(1,3))));
    RS_Graphic* group2 = new RS_Graphic();
    RS_Graphic* group21 = new RS_Graphic();
    group21->addEntity(new RS_Line(group21,
                                   RS_LineData(RS_Vector(21,1), RS_Vector(21,1))));
    group21->addEntity(new RS_Line(group21,
                                   RS_LineData(RS_Vector(21,2), RS_Vector(21,2))));
    group2->addEntity(group21);
    group2->addEntity(new RS_Line(group2,
                                  RS_LineData(RS_Vector(2,1), RS_Vector(2,1))));
    group2->addEntity(new RS_Line(group2,
                                  RS_LineData(RS_Vector(2,2), RS_Vector(2,2))));
    group2->addEntity(new RS_Line(group2,
                                  RS_LineData(RS_Vector(2,3), RS_Vector(2,3))));
    group2->addEntity(new RS_Line(group2,
                                  RS_LineData(RS_Vector(2,4), RS_Vector(2,4))));

    graphic.addEntity(new RS_Point(&graphic, RS_PointData(RS_Vector(50,50))));
    graphic.addEntity(group1);
    graphic.addEntity(group2);

    std::cout << graphic;

    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "Testing hierarchical entities done\n\n");
}


/**
 * Test routine for measuring distances.
 */
void testDistanceLine() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting Distance to line");

    RS_Line line(NULL,
                 RS_LineData(RS_Vector(10,10), RS_Vector(100,10)));
    std::cout << line;

    for (double y=0.0; y<50.0; y+=5.0) {
        RS_Vector v(50, y);
        double d = line.getDistanceToPoint(v, NULL);
        std::cout << v << "\n";
        RS_DEBUG->print("distance: %f", d);
    }
}

void testNearestPointOnEntity() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nNearest point on entity");

    double dist;
    RS_Line line(NULL,
                 RS_LineData(RS_Vector(10,10), RS_Vector(100,10)));
    RS_Vector v(9, 15);

    std::cout << line;
    std::cout << "Point: " << v << "\n";

    RS_Vector p = line.getNearestPointOnEntity(v, &dist);

    std::cout << "Nearest Point: " << p << "\n";
}


/**
 * Test routine for DXF import.
 */
void testDXFImport() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting DXF import");

    RS_Graphic graphic;

    RS_FILEIO->fileImport(graphic, "demo.dxf");

    std::cout << graphic;

    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "Testing DXF import done\n\n");

}


/**
 * Test routine for DXF export.
 */
void testDXFExport() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting DXF export");

    RS_Graphic graphic;
    //RS_Creation creation(graphic);

    RS_Polyline *pl = new RS_Polyline(&graphic,
                                      RS_PolylineData());
    pl->addVertex(RS_Vector(1.0, 5.0), 0.0);
    pl->addVertex(RS_Vector(10.0, 10.0), 0.0);
    pl->addVertex(RS_Vector(50.0, 15.0), 0.0);
    pl->addVertex(RS_Vector(100.0, 0.0), 0.0);

    graphic.addEntity(pl);
    RS_FILEIO->fileExport(graphic, "export_demo.dxf");

    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "Testing DXF export done\n\n");
}


/**
 * Test routine for Math functions.
 */
void testMath() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting Math");
    //RS_Arc arc(NULL, RS_Vector(12.3, 3.4), 50.0, 10.0, 50.0, false);

    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting RS_Math::isAngleBetween");
    double a = 12.5;
    double a1, a2;
    for (a1=0.0, a2=0.0; a1<360.0 && a2<360.0; a1+=2.5, a2+=7.0) {
        RS_DEBUG->print("angle %f is %s between %f and %f",
                        a, RS_Math::isAngleBetween(a, a1, a2, false) ? "" : "not",
                        a1, a2);
    }
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "Testing RS_Math::isAngleBetween done\n\n");

    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting RS_Vector::angleTo");

    RS_Vector v1(14.5, 35.6);

    for (a1=0.0; a1<360.0; a1+=2.5) {
        RS_Vector v2(v1.x + cos(a1/ARAD)*50.0, v1.y + sin(a1/ARAD)*50.0);

        RS_DEBUG->print("angle from %f/%f to %f/%f is %f",
                        v1.x, v1.y, v2.x, v2.y, v1.angleTo(v2));
    }
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "Testing RS_Vector::angleTo done\n\n");


    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "Testing Math done\n\n");
}


void testBlocks() {
    /*
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting Blocks");
    // create a graphic:
    RS_Graphic* graphic = new RS_Graphic();

    // create a block:
    RS_Block* block = new RS_Block(graphic, "myblock01",
                                   RS_Vector(0.0,0.0));
    block->addEntity(new RS_Line(block,
                                 RS_LineData(RS_Vector(5.0, 0.0), RS_Vector(100.0, 40.0))));

    // add the block to the blocklist of the graphic:
    graphic->addBlock(block);

    // insert the block into the graphic:
    RS_Insert* ins = new RS_Insert(graphic,
                                   RS_InsertData("myblock01",
                                                 RS_Vector(10.0, 20.0),
                                                 RS_Vector(1.0, 1.0),
                                                 0.0,
                                                 1, 1,
                                                 RS_Vector(0.0, 0.0)));
    graphic->addEntity(ins);

    std::cout << "We'll save this Graphic: \n\n" << *graphic << "\n\n";

    RS_Export exp(*graphic);
    exp.fileExport("blocks.dxf");

    RS_Graphic* graphicIn = new RS_Graphic();

    RS_Import import(*graphicIn);
    import.fileImport("blocks.dxf");

    std::cout << "We reloaded this Graphic: \n\n" << *graphicIn << "\n\n";
    delete graphic;
    */
}

void testCloning() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting Clones");
   
    /*
    RS_EntityContainer* g1 = new RS_EntityContainer();
    RS_Entity* e1 = new RS_Line(g1,
                                RS_LineData(RS_Vector(2.0, 3.0), RS_Vector(50.0, 40.0)));

    std::cout << "original: " << *e1;
    RS_Entity* e2 = e1->clone();
    std::cout << "clone: " << *e2;

    std::cout << "modifying clone...\n\n";
    ((RS_Line*)e2)->setStartpoint(RS_Vector(2.5, 4.5));
    std::cout << "original: " << *e1;
    std::cout << "clone: " << *e2;

    std::cout << "cloning graphic...\n\n";
    g1->addEntity(e1);
    std::cout << "original: " << *g1;
    RS_EntityContainer* g2 = (RS_EntityContainer*)g1->clone();
    std::cout << "clone: " << *g2;

    std::cout << "modifying clone...\n\n";
    RS_Line* l = (RS_Line*)g2->firstEntity();
    l->setStartpoint(RS_Vector(3.3, 7.7));
    std::cout << "original: " << *g1;
    std::cout << "clone: " << *g2;

    delete g1;
    delete g2;
    */

	RS_EntityContainer* currentEC = new RS_EntityContainer();
	for (int i=0; i<2000; ++i) {
	    RS_Entity* e1 = new RS_Line(currentEC,
	           RS_LineData(RS_Vector(2.0, 3.0), RS_Vector(50.0, 40.0)));
		currentEC->addEntity(e1);
	}

	RS_EntityContainer* cloneEC;
	for (int k=0; k<10000; ++k) {
	    RS_EntityContainer* cloneEC = (RS_EntityContainer*)currentEC->clone();
        delete currentEC;
        currentEC = cloneEC;
	}

    getchar();
}


void testIntersection() {
    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n\nTesting Intersections");

    RS_DEBUG->print(RS_Debug::D_DEBUGGING, "\n  Between two lines:");

    RS_VectorSolutions s;

    RS_EntityContainer* g1 = new RS_EntityContainer();
    RS_Entity* e1 = new RS_Line(g1,
                                RS_LineData(RS_Vector(10.0, 30.0),
                                            RS_Vector(40.0, 50.0)));
    g1->addEntity(e1);

    // line / line:
    std::cout << "line / line:\n";
    std::cout << "parallel:\n";
    RS_Entity* e2 = new RS_Line(g1,
                                RS_LineData(RS_Vector(20.0, 10.0),
                                            RS_Vector(50.0, 30.0)));
    g1->addEntity(e2);
    s = RS_Information::getIntersection(e1, e2, true);
    std::cout << "e1, e2:\n" << s << "\n\n";

    std::cout << "connecting endpoints:\n";
    RS_Entity* e3 = new RS_Line(g1,
                                RS_LineData(RS_Vector(40.0, 50.0),
                                            RS_Vector(70.0, 100.0)));
    g1->addEntity(e3);
    s = RS_Information::getIntersection(e1, e3, true);
    std::cout << "e1, e3:\n" << s << "\n\n";

    std::cout << "endpoint touches line:\n";
    RS_Entity* e4 = new RS_Line(g1,
                                RS_LineData(RS_Vector(50.0, 40.0),
                                            RS_Vector(30.0, 60.0)));
    g1->addEntity(e4);
    s = RS_Information::getIntersection(e1, e4, true);
    std::cout << "e1, e4:\n" << s << "\n\n";

    std::cout << "just not intersecting:\n";
    RS_Entity* e5 = new RS_Line(g1,
                                RS_LineData(RS_Vector(10.0, 40.0),
                                            RS_Vector(20.0, 60.0)));
    g1->addEntity(e5);
    s = RS_Information::getIntersection(e1, e5, true);
    std::cout << "e1, e5:\n" << s << "\n\n";

    std::cout << "intersecting:\n";
    RS_Entity* e6 = new RS_Line(g1,
                                RS_LineData(RS_Vector(30.0, 30.0),
                                            RS_Vector(20.0, 50.0)));
    g1->addEntity(e6);
    s = RS_Information::getIntersection(e1, e6, true);
    std::cout << "e1, e6:\n" << s << "\n\n";


    // line / arc:
    std::cout << "line / arc:\n";
    RS_Entity* e7 = new RS_Arc(g1,
                               RS_ArcData(RS_Vector(30.0,30.0), 15.0, 0.0, 2*M_PI, false));
    s = RS_Information::getIntersection(e1, e7, true);
    std::cout << "e1, e7:\n" << s << "\n\n";
}


void usage () {
    //std::cout << "\nUsage: demo <DXF file>\n\n";
}


// EOF
