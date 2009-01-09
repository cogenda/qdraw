/****************************************************************************
**
** Copyright (C) 2007-2008 Cogenda. All rights reserved.
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
**********************************************************************/

#ifndef __cg_meshgen_h__
#define __cg_meshgen_h__

#include <vector>
#include <map>
#include <string>
#include "triangle.h"

class RS_String;
class RS_Vector;
class RS_Document;
class RS_Document;
class RS_GraphicView;

class MeshGenerator
{
public:
 /**
  * constructor, build PSLG from  RS_EntityContainer
  */
  MeshGenerator(RS_Document * doc, RS_GraphicView * gv);

  /**
   * des, free triangle io
   */
  virtual ~MeshGenerator();
  
  /**
   * generate mesh with PSLG
   */
  virtual void do_mesh(const QString &cmd);

  virtual void draw_mesh();

  /**
   * export mesh as vtk ascii file format
   */ 
  virtual void export_mesh_vtk(const char * name);

private:

 /**
  * we will add mesh to RS_Document later
  */
  RS_Document * _doc;

 /**
  * we only mesh objects on RS_GraphicView
  * and add final mesh to RS_GraphicView for display.
  */
  RS_GraphicView * _gv;

  /**
   * function to add point into PSLG, drop any overlaped points
   */
  unsigned int add_point(const RS_Vector &v);

  /**
   * PSLG points
   */
  std::vector<RS_Vector> _points;

  struct CG_Segment
  {
     unsigned int p1, p2;
     int mark;
  };

  /**
   * PSLG segments
   */
  std::vector<CG_Segment> _segments;

  /**
   * map mark to segment label
   */
  std::map<RS_String, int> _label_to_mark;

  struct CG_Region
  {
    bool hole;
    double x;
    double y;
    double area_control; 
    RS_String label;
    RS_String material;
  };

  /**
   * PSLG region(hole)
   */
  std::vector<CG_Region> _regions;


  /**
   * triangle data structure
   */
  triangulateio in, out;

  void triangulateio_init();

  void triangulateio_finalize();

};

#endif // #define __cg_meshgen_h__

