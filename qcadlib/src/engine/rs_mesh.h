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

/**
 * Class representing a mesh.
 *
 * @author Gong Ding
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

  void export_mesh(const RS_String & file);

private:

  /**
   *  mesh data is here
   */
  triangulateio io;

  /**
   * segment mark to segment label (in QString)
   */
  std::map<int , RS_String> _segment_mark_to_label;

  /**
   * segment mark to region label and material
   */
  std::map<int, std::pair<RS_String, RS_String> > _region_mark_to_label_material;

  RS_String _tri_cmd;
};


#endif
