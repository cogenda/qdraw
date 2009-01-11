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


#include "rs_entitycontainer.h"

/**
 * Class representing a mesh.
 *
 * @author Gong Ding
 */
class RS_Mesh : public RS_EntityContainer
{

public:

  RS_Mesh(RS_EntityContainer* parent=NULL, bool owner=true)
  :RS_EntityContainer(parent, owner)
  {}

  virtual ~RS_Mesh()
  {}

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

  //virtual void set_mesh();
  //virtual void export_mesh(char *);
private:

  // mesh data is here
}
;


#endif
