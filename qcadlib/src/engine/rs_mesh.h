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

#include <map>

#include "triangle.h"
#include "rs_entitycontainer.h"

class ProfileManager;

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

  triangulateio & get_triangulateio()
  { return io; }

  const triangulateio & get_triangulateio() const
  { return io; }

  void add_segment_info(int mark, RS_String label)
  { _segment_mark_to_label[mark] = label;  }

  void add_region_info(int mark, RS_String label, RS_String material)
  { _region_mark_to_label_material[mark] = std::make_pair(label, material); }

  const RS_String & tri_cmd() const
  {return _tri_cmd;}

  RS_String & tri_cmd()
  {return _tri_cmd;}

  void set_profile_manager(ProfileManager *pm)
  { _pm = pm; }

  void set_refine_flag(double d, bool signed_log);

  void export_mesh(const RS_String & file);

  void clear();

private:

  /**
   *  mesh data is here
   */
  triangulateio io;

  void clear_triangulateio();

  double max_dispersion(int a, int b, int c, bool signed_log);

  double triangle_area(int a, int b, int c);

  /**
   * segment mark to segment label (in QString)
   */
  std::map<int , RS_String> _segment_mark_to_label;

  /**
   * segment mark to region label and material
   */
  std::map<int, std::pair<RS_String, RS_String> > _region_mark_to_label_material;

  /**
   * Triangle command line switch
   */
  RS_String _tri_cmd;

  /**
   * hold a pointer to ProfileManager
   */
  ProfileManager *_pm;
};


#endif
