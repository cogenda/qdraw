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
class RS_Mesh;
class ProfileManager;

class MeshGenerator
{
public:
 /**
  * constructor, hold RS_Document and RS_GraphicView pointer
  */
  MeshGenerator(RS_Document * doc, RS_GraphicView * gv, ProfileManager * pm);

  /**
   * des, free triangle in
   */
  virtual ~MeshGenerator();

  /**
   * generate mesh with PSLG
   */
  virtual void do_mesh(const QString &cmd);

  /**
   * refine existing mesh
   */
  virtual void refine_mesh(const QString &cmd);

  /**
   * export mesh as vtk ascii file format, debug only
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
   * build PSLG from visitable Entities in current layer
   */
  void convert_cad_to_pslg();

  /**
   * function to add point into PSLG, drop any overlaped points
   */
  unsigned int add_point(const RS_Vector &v);

  /**
   * draw mesh to the graph
   */
  RS_Mesh* draw_mesh();

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
    double x;
    double y;
    double area_control;
    RS_String label;
    RS_String material;
  };

  /**
   * PSLG region
   */
  std::vector<CG_Region> _regions;

  struct CG_Hole
  {
    double x;
    double y;
  };

  /**
   * PSLG hole
   */
  std::vector<CG_Hole> _holes;

  /**
   * triangle data structure
   */
  triangulateio in, out;

  void triangulateio_init();

  void triangulateio_finalize();

  void triangulateio_copy(const triangulateio & src, triangulateio & dst);

  /**
   * hold a pointer to ProfileManager
   */
  ProfileManager *_pm;
};

#endif // #define __cg_meshgen_h__

