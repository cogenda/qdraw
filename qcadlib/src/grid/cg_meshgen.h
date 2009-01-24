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


#include <string>
#include "triangle.h"
#include "cg_pslg.h"


class RS_String;
class RS_Vector;
class RS_Document;
class RS_Document;
class RS_GraphicView;
class RS_Mesh;
class QuadTree;
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
  virtual void do_mesh(const QString &cmd, bool enable_quadtree);

  /**
   * refine existing mesh
   */
  virtual void refine_mesh(const QString &cmd, double max_d, bool signed_log, bool enable_quadtree);

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
   * PSLG for mesh
   */
  CG_PSLG * _pslg;

  /**
   * create a new layer to hold the mesh
   */
  void create_new_mesh_layer();

  /**
   * find existing mesh object
   */
  RS_Mesh* find_mesh();

  /**
   * use quadtree method to create aux mesh points
   */
  QuadTree * build_quadtree();

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

