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

#ifndef __cg_export_mesh_h__
#define __cg_export_mesh_h__

#include "rs.h"

class RS_GraphicView;
class RS_Mesh;

/**
 * Mesh Export dialogs.
 */
class CG_MeshExport
{
public:
  CG_MeshExport(QWidget* parent, RS_GraphicView * gv);

  static QString getSaveFileName(QWidget* parent);

  void write_mesh_to_tif();

private:

  QWidget * _parent;

  RS_GraphicView * _gv;

  RS_Mesh * _mesh;

  RS_Mesh * find_mesh_obj();
};

#endif
