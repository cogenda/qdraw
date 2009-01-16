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

#include "rs_material.h"

std::map<RS_String, RS_Material::MaterialType> RS_Material::material_name_to_material_type;

// all the supported materials are listed here
// one can add more later
void RS_Material::init_material_name_to_material_type()
{
  if( material_name_to_material_type.size() ) return;

  material_name_to_material_type["Si"     ]  = Semiconductor;
  material_name_to_material_type["Silicon"]  = Semiconductor;
  material_name_to_material_type["Ge"     ]  = Semiconductor;
  material_name_to_material_type["GaAs"   ]  = Semiconductor;
  material_name_to_material_type["InAs"   ]  = Semiconductor;
  material_name_to_material_type["InSb"   ]  = Semiconductor;
  material_name_to_material_type["InP"    ]  = Semiconductor;
  material_name_to_material_type["InN"    ]  = Semiconductor;
  material_name_to_material_type["SiGe"   ]  = SingleCompoundSemiconductor;
  material_name_to_material_type["AlGaAs" ]  = SingleCompoundSemiconductor;
  material_name_to_material_type["InGaAs" ]  = SingleCompoundSemiconductor;
  material_name_to_material_type["HgCdTe" ]  = SingleCompoundSemiconductor;
  material_name_to_material_type["AlInAs" ]  = SingleCompoundSemiconductor;
  material_name_to_material_type["GaAsP"  ]  = SingleCompoundSemiconductor;
  material_name_to_material_type["InGaP"  ]  = SingleCompoundSemiconductor;
  material_name_to_material_type["InAsP"  ]  = SingleCompoundSemiconductor;
  material_name_to_material_type["3C-SiC" ]  = Semiconductor;
  material_name_to_material_type["S-SiO2" ]  = Semiconductor;

  material_name_to_material_type["Ox"     ]  = Insulator;
  material_name_to_material_type["SiO2"   ]  = Insulator;
  material_name_to_material_type["Nitride"]  = Insulator;
  material_name_to_material_type["Si3N4"  ]  = Insulator;
  material_name_to_material_type["Nit"    ]  = Insulator;

  material_name_to_material_type["Air"    ]  = Insulator;

  material_name_to_material_type["Elec"   ]  = Conductor;
  material_name_to_material_type["Al"     ]  = Conductor;
  material_name_to_material_type["Ag"     ]  = Conductor;
  material_name_to_material_type["Silver" ]  = Conductor;
  material_name_to_material_type["Au"     ]  = Conductor;
  material_name_to_material_type["Gold"   ]  = Conductor;
  material_name_to_material_type["Cu"     ]  = Conductor;
  material_name_to_material_type["Copper" ]  = Conductor;
  material_name_to_material_type["PolySi" ]  = Conductor;
  material_name_to_material_type["TiSi2"  ]  = Conductor;

  material_name_to_material_type["Vacuum" ]  = Vacuum;

  material_name_to_material_type["PML"    ]  = PML;

  material_name_to_material_type["Invalid"]  = INVALID_MATERIAL_TYPE;
}



std::vector<RS_String> RS_Material::get_materials()
{
  std::vector<RS_String> materials;
  materials.push_back("Invalid");
  materials.push_back("Si"     );
  materials.push_back("Silicon");
  materials.push_back("Ge"     );
  materials.push_back("GaAs"   );
  materials.push_back("InAs"   );
  materials.push_back("InSb"   );
  materials.push_back("InP"    );
  materials.push_back("InN"    );
  materials.push_back("SiGe"   );
  materials.push_back("AlGaAs" );
  materials.push_back("InGaAs" );
  materials.push_back("HgCdTe" );
  materials.push_back("AlInAs" );
  materials.push_back("GaAsP"  );
  materials.push_back("InGaP"  );
  materials.push_back("InAsP"  );
  materials.push_back("3C-SiC" );
  materials.push_back("S-SiO2" );

  materials.push_back("Ox"     );
  materials.push_back("SiO2"   );
  materials.push_back("Nitride");
  materials.push_back("Si3N4"  );
  materials.push_back("Nit"    );

  materials.push_back("Air"    );

  materials.push_back("Elec"   );
  materials.push_back("Al"     );
  materials.push_back("Ag"     );
  materials.push_back("Silver" );
  materials.push_back("Au"     );
  materials.push_back("Gold"   );
  materials.push_back("Cu"     );
  materials.push_back("Copper" );
  materials.push_back("PolySi" );
  materials.push_back("TiSi2"  );

  materials.push_back("Vacuum" );

  materials.push_back("PML"    );

  return materials;
}


bool RS_Material::IsSemiconductor(const RS_String & mat_name)
{
  init_material_name_to_material_type();
  if ( material_name_to_material_type[mat_name] == Semiconductor                  ||
       material_name_to_material_type[mat_name] == SingleCompoundSemiconductor    ||
       material_name_to_material_type[mat_name] == ComplexCompoundSemiconductor
     )
    return true;

  return false;
}


bool RS_Material::IsInsulator(const RS_String & mat_name)
{
  init_material_name_to_material_type();
  if ( material_name_to_material_type[mat_name] == Insulator )
    return true;

  return false;
}

bool RS_Material::IsConductor(const RS_String & mat_name)
{
  init_material_name_to_material_type();
  if ( material_name_to_material_type[mat_name] == Conductor )
    return true;

  return false;
}
