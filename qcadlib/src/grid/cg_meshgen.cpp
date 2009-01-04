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

#include "rs_string.h"
#include "rs_entitycontainer.h"
#include "cg_meshgen.h"



MeshGenerator::MeshGenerator(RS_EntityContainer * entities):_entities(entities)
{
  //mesh all the entity on active layer
  for (const RS_Entity* e=_entities->firstEntity(RS2::ResolveNone); e!=NULL; e=_entities->nextEntity(RS2::ResolveNone))
    if(e->isOnActiveLayer())
    {
      switch (e->rtti())
      {
      case RS2::EntityPoint :
        {
          const RS_Point * p = (const RS_Point *)e;
          add_point(p->getData().pos);
          break;
        }
      case RS2::EntityLine :
        {
          const RS_Line * line = (const RS_Line *)e;
          CG_Segment segment;
          segment.p1 = add_point(line->getStartpoint());
          segment.p2 = add_point(line->getEndpoint());

          if(_mark_to_label.find(line->getLabel())!=_mark_to_label.end())
            segment.mark = _mark_to_label[line->getLabel()];
          else
          {
            segment.mark = _mark_to_label.size();
            _mark_to_label[line->getLabel()] = segment.mark;
          }
          _segments.push_back(segment);
          break;
        }
      default: break;
      }
    }
}

unsigned int MeshGenerator::add_point(const RS_Vector &v)
{
  for(unsigned int n=0; n<_points.size(); ++n)
    if( v.distanceTo(_points[n])<1e-6 ) return n;
  _points.push_back(v);
  return _points.size()-1;
}


void MeshGenerator::do_mesh()
{





}


void MeshGenerator::export_mesh(const char * name)
{
  /*
  std::ofstream out;
  out.open(name, std::ofstream::trunc);

  out << "# vtk DataFile Version 3.0" <<'\n';
  out << "Date calculated by CTRI"    <<'\n';
  out << "ASCII"                      <<'\n';
  out << "DATASET UNSTRUCTURED_GRID"  <<'\n';
  out << "POINTS " << _points.size()  << " float" << '\n';

  for (unsigned int n=0; n<_points.size(); ++n)
  {
  	Point & p = *_points[n];
  	out << p[0] << " " << p[1] << " " << 0.0 << '\n';
  }

  out << std::endl;

  out<<"CELLS "<<_triangles.size()<<" "<<4*_triangles.size()<<'\n';

  for(TriangleIt it=_triangles.begin(); it!=_triangles.end(); ++it)
  {
  	out << 3 << " "
  			<< (*it)->get_point(0)->id() << " "
  			<< (*it)->get_point(1)->id() << " "
  			<< (*it)->get_point(2)->id() << "\n";
  }

  out << std::endl;

  out << "CELL_TYPES " << _triangles.size() << '\n';

  for(unsigned int n=0; n<_triangles.size(); ++n)
  	out << 5 << std::endl;

  out << std::endl;

  out << "CELL_DATA "<<_triangles.size()     <<'\n';

  out << "SCALARS state float 1"  <<'\n';
  out << "LOOKUP_TABLE default"    <<'\n';
  for(TriangleIt it=_triangles.begin(); it!=_triangles.end(); ++it)
  {
  	out << (*it)->state()<<'\n';
  }
  out<<std::endl;


  out << "SCALARS material float 1"  <<'\n';
  out << "LOOKUP_TABLE default"    <<'\n';
  for(TriangleIt it=_triangles.begin(); it!=_triangles.end(); ++it)
  {
  	out << (*it)->material()<<'\n';
  }
  out<<std::endl;


  out << "SCALARS mark float 1"  <<'\n';
  out << "LOOKUP_TABLE default"    <<'\n';
  for(TriangleIt it=_triangles.begin(); it!=_triangles.end(); ++it)
  {
  	out << (*it)->mark()<<'\n';
  }
  out<<std::endl;


     //write node based boundary info to vtk
  out<<"POINT_DATA "<< _points.size()      <<'\n';
  out<<"SCALARS point_mark float 1" <<'\n';
  out<<"LOOKUP_TABLE default"       <<'\n';
  for(unsigned int n=0; n<_points.size(); ++n)
  {
  	out<<_points[n]->mark()<<'\n' ;
  }
  out<<std::endl;

  out.close();

  */
}
