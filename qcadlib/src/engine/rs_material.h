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

#ifndef RS_MATERIAL_H
#define RS_MATERIAL_H

#include <map>
#include <vector>

#include "rs_string.h"

class RS_Material
{
public:

  enum  MaterialType
  {
    Vacuum                      = 0,
    Semiconductor                  ,
    SingleCompoundSemiconductor    ,
    ComplexCompoundSemiconductor   ,
    Conductor                      ,
    Insulator                      ,
    PML                            ,
    INVALID_MATERIAL_TYPE            // should always be last
  };

  static std::vector<RS_String> get_materials();
  static bool IsSemiconductor(const RS_String & mat_name);
  static bool IsInsulator(const RS_String & mat_name);
  static bool IsConductor(const RS_String & mat_name);

private:

  static void init_material_name_to_material_type();

  static std::map<RS_String, MaterialType> material_name_to_material_type;
};


#endif
