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


#ifndef RS_MESH_H
#define RS_MESH_H

#include <vector>
#include <map>

#include "triangle.h"
#include "cg_pslg.h"
#include "cg_quadtree.h"
#include "rs_hatch.h"
#include "rs_entitycontainer.h"

class ProfileManager;
class QuadTree;

/**
 * Class representing a mesh.
 *
 * @author Gong Ding  gdiso@cogenda.com
 */
class RS_Mesh : public RS_EntityContainer
{

public:

  RS_Mesh(RS_EntityContainer* parent=NULL, bool owner=true);

  virtual ~RS_Mesh();

  virtual void initLabel()
  {
    static unsigned long int idCounter=0;
    label = "Mesh" + RS_String::number(idCounter++);
  }

  /** @return RS2::EntityMesh */
  virtual RS2::EntityType rtti() const
  {
    return RS2::EntityMesh;
  }

  virtual void update();

  virtual void draw(RS_Painter*, RS_GraphicView*,  double );

  bool draw_outline() const
  { return _draw_outline;}

  bool & draw_outline()
  { return _draw_outline;}

  bool draw_contour() const
  { return _draw_contour; }

  bool & draw_contour()
  { return _draw_contour; }

  bool  draw_material() const
  { return _draw_material; }

  bool & draw_material()
  { return _draw_material; }

  bool draw_mesh() const
  { return _draw_mesh; }

  bool & draw_mesh()
  { return _draw_mesh; }

  int contour_number() const
  { return _contour_number; }

  int & contour_number()
  { return _contour_number; }

  bool contour_with_signed_log() const
  { return _use_signed_log; }

  bool & contour_with_signed_log()
  { return _use_signed_log; }

  triangulateio & get_triangulateio()
  { return io; }

  const triangulateio & get_triangulateio() const
  { return io; }

  const RS_String & tri_cmd() const
  {return _tri_cmd;}

  RS_String & tri_cmd()
  {return _tri_cmd;}

  void set_profile_manager(ProfileManager *pm)
  { _pm = pm; }

  void set_pslg(CG_PSLG * pslg)
  { _pslg = pslg; }

  CG_PSLG * get_pslg()
  { return _pslg; }

  void set_quadtree(QuadTree * qt)
  { _quadtree = qt; }

  QuadTree * get_quadtree()
  { return _quadtree; }

  void set_refine_flag(double d, bool signed_log);

  bool is_refine_required(const QuadTree::iterator &it, double d, bool signed_log);

  void export_mesh(const RS_String & file);

  void clear();

private:

  /**
   *  mesh data is here
   */
  triangulateio io;

  void clear_triangulateio();

  double triangle_area_constraint(int a, int b, int c, double dmax, bool signed_log);

  double triangle_area(int a, int b, int c);

  RS_Hatch * create_triangle(RS_Vector a, RS_Vector b, RS_Vector c);
  RS_Hatch * create_rectangle(RS_Vector a, RS_Vector b, RS_Vector c, RS_Vector d);

  /**
   * the PSLG of this mesh
   */
  CG_PSLG * _pslg;

  /**
   * the quadtree struct
   */
  QuadTree * _quadtree;

  /**
   * Triangle command line switch
   */
  RS_String _tri_cmd;

  /**
   * hold a pointer to ProfileManager
   */
  ProfileManager *_pm;

  double profile(double x, double y);

  RS_Vector linear_interpolation(RS_Vector a, RS_Vector b, double level);

  static const int rainbow_color_table[19][3];

  bool _draw_outline;

  bool _draw_mesh;

  bool _draw_contour;

  bool _draw_material;

  bool _use_signed_log;

  int  _contour_number;

};


#endif
