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

#include <ctime>
#include <fstream>

#include "rs_mesh.h"

RS_Mesh::RS_Mesh(RS_EntityContainer* parent, bool owner)
  :RS_EntityContainer(parent, owner)
{
  io.numberofpoints = 0;
  io.pointlist = (double *) NULL;
  io.pointattributelist = (double *) NULL;
  io.pointmarkerlist = (int *) NULL;

  io.numberoftriangles = 0;
  io.trianglelist = (int *) NULL;
  io.triangleattributelist = (double *) NULL;
  io.trianglearealist = (double *) NULL;

  io.numberofsegments = 0;
  io.segmentlist = (int *) NULL;
  io.segmentmarkerlist = (int *) NULL;
  io.numberofregions = 0;
  io.numberofholes = 0;
  io.regionlist = (double *)NULL;
  io.holelist = (double *)NULL;
}


RS_Mesh::~RS_Mesh()
{
  free(io.pointlist);
  free(io.pointmarkerlist);
  free(io.pointattributelist);

  free(io.trianglelist);
  free(io.triangleattributelist);
  free(io.trianglearealist);

  free(io.segmentlist);
  free(io.segmentmarkerlist);

  free(io.regionlist);
  free(io.holelist);
}


void RS_Mesh::export_mesh(const RS_String & file)
{
  if(!io.numberofpoints || !io.numberoftriangles) return;

  std::ofstream fout;
  fout.open(file.ascii(), std::ofstream::trunc);

  time_t          _time;
  time(&_time);

  fout << "h TIF V1.2.1 created by SMESH, Copyright (C) by Cogenda Inc. Date: " << ctime(&_time)  << '\n';
  fout << "cd GEN          blnk               blnk          blnk        cart2D    1.00000E+00  0.00000E+00"  << '\n';
  fout << "cg   3.00000E+02" << '\n';

  //write point
  for(int i=0; i<io.numberofpoints; ++i)
    fout<< 'c' << '\t'
        << i+1 << '\t'
        << io.pointlist[2*i+0] << '\t' << '\t'
        << io.pointlist[2*i+1] << '\t' << '\t'
        << io.pointmarkerlist[i] << '\n';
  fout<<'\n';

  //write edge
  for(int i=0; i<io.numberofsegments; ++i)
    fout<< 'e' << '\t'
        << i+1 << '\t'
        << io.segmentlist[2*i+0] + 1 << '\t'
        << io.segmentlist[2*i+1] + 1 << '\t'
        << 0 << '\n';
  fout<<'\n';

  // write region information
  std::map<int, std::pair<RS_String, RS_String> >::iterator region_it = _region_mark_to_label_material.begin();
  for(; region_it!=_region_mark_to_label_material.end(); ++region_it)
    fout<< 'r' << '\t'
        << region_it->first+1 << '\t'
        << (region_it->second).second << '\t'
        << (region_it->second).first << '\n';
  fout<<'\n';

  // write segment
  std::map<int , RS_String>::iterator segment_it = _segment_mark_to_label.begin();
  for(; segment_it != _segment_mark_to_label.end(); ++segment_it)
  {
    fout<< 'i' << '\t'
        << segment_it->first + 1 << '\t'
        << "ANY" << '\t'
        << segment_it->second << '\t'
        << 0 << '\n';
    for(int i=0; i<io.numberofsegments; ++i)
      if(io.segmentmarkerlist[i] == segment_it->first)
        fout<< " j" << '\t'
            <<  i+1 << '\n';
  }
  fout<<'\n';

  //write triangles
  for(int i=0; i<io.numberoftriangles; ++i)
    fout<< 't' << '\t'
        << i+1 << '\t'
        << int(io.triangleattributelist[i]+0.5)+1 << '\t'
        << io.trianglelist[3*i+0]+1 << '\t'
        << io.trianglelist[3*i+1]+1 << '\t'
        << io.trianglelist[3*i+2]+1 << '\t'
        << 0 << '\t'
        << 0 << '\t'
        << 0 << '\n';
  fout<<'\n';

  // write profile information

  fout.close();
}
