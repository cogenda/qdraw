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

#include <string.h>
#include <time.h>
#include <fstream>
#include <set>

#include "rs_settings.h"
#include "rs_units.h"
#include "rs_mesh.h"
#include "rs_graphic.h"
#include "rs_graphicview.h"
#include "cg_profile.h"
#include "cg_profile_manager.h"



RS_Mesh::RS_Mesh(RS_EntityContainer* parent, bool owner)
    :RS_EntityContainer(parent, owner), _pm(0)
{
  memset(&io, 0, sizeof(triangulateio));
}


RS_Mesh::~RS_Mesh()
{
  clear_triangulateio();
}


void RS_Mesh::clear()
{
  clear_triangulateio();
  RS_EntityContainer::clear();
}



void RS_Mesh::clear_triangulateio()
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

  memset(&io, 0, sizeof(triangulateio));
}


void RS_Mesh::set_refine_flag(double dmax, bool signed_log)
{

  io.numberofcorners = 3;
  io.trianglearealist = (double *) calloc(io.numberoftriangles, sizeof(double));
  for(int i=0; i<io.numberoftriangles; i++)
  {
    int a = io.trianglelist[3*i+0];
    int b = io.trianglelist[3*i+1];
    int c = io.trianglelist[3*i+2];
    io.trianglearealist[i] = triangle_area_constraint(a, b, c, dmax, signed_log);
  }
}


double RS_Mesh::triangle_area(int a, int b, int c)
{
  double x1 = io.pointlist[2*a+0];
  double y1 = io.pointlist[2*a+1];

  double x2 = io.pointlist[2*b+0];
  double y2 = io.pointlist[2*b+1];

  double x3 = io.pointlist[2*c+0];
  double y3 = io.pointlist[2*c+1];

  double d1 = sqrt((x3-x2)*(x3-x2) +(y3-y2)*(y3-y2));
  double d2 = sqrt((x1-x3)*(x1-x3) +(y1-y3)*(y1-y3));
  double d3 = sqrt((x1-x2)*(x1-x2) +(y1-y2)*(y1-y2));
  double d = 0.5*(d1+d2+d3);

  return sqrt(d*(d-d1)*(d-d2)*(d-d3));
}


double RS_Mesh::triangle_area_constraint(int a, int b, int c, double dmax, bool signed_log)
{
  double Scale = 1.1;
  double area  = triangle_area(a, b, c);
  double xa = io.pointlist[2*a+0];
  double ya = io.pointlist[2*a+1];

  double xb = io.pointlist[2*b+0];
  double yb = io.pointlist[2*b+1];

  double xc = io.pointlist[2*c+0];
  double yc = io.pointlist[2*c+1];

  double pa = fabs(_pm->profile("Nd", xa, ya)) - fabs(_pm->profile("Na", xa, ya));
  double pb = fabs(_pm->profile("Nd", xb, yb)) - fabs(_pm->profile("Na", xb, yb));
  double pc = fabs(_pm->profile("Nd", xc, yc)) - fabs(_pm->profile("Na", xc, yc));

  double doping = fabs(pa+pb+pc)/3.0;

  if(signed_log)
  {
    pa = (pa > 0 ? 1.0 : -1.0) * log(fabs(pa) + 1);
    pb = (pb > 0 ? 1.0 : -1.0) * log(fabs(pb) + 1);
    pc = (pc > 0 ? 1.0 : -1.0) * log(fabs(pc) + 1);
  }

  double dispersion =  std::max(std::max(fabs(pa-pb),fabs(pb-pc)),fabs(pc-pa))+1e-6;

  double eps  = 8.85e-12;
  double kb   = 1.3806503e-23;
  double e    = 1.602176462e-19;
  double Ld   = sqrt(13*eps*kb*300/(e*e*doping));

  if(dispersion>dmax)
    Scale = Ld*Ld/(2*area) < dmax/dispersion ? Ld*Ld/(2*area) : dmax/dispersion;

  if(Scale<0.25)       Scale = 0.25;

  return area*Scale;
}


void RS_Mesh::update()
{
  //clear old entities
  entities.clear();
  resetBorders();


  //build the bound box
  RS_Vector bbox_min( RS_MAXDOUBLE,  RS_MAXDOUBLE,  RS_MAXDOUBLE);
  RS_Vector bbox_max(-RS_MAXDOUBLE, -RS_MAXDOUBLE, -RS_MAXDOUBLE);
  for(int i=0; i<io.numberofpoints; ++i)
  {
    double z   = _pm->profile(io.pointlist[2*i], io.pointlist[2*i+1]);
    bbox_min.x = bbox_min.x > io.pointlist[2*i  ] ? io.pointlist[2*i  ] : bbox_min.x;
    bbox_min.y = bbox_min.y > io.pointlist[2*i+1] ? io.pointlist[2*i+1] : bbox_min.y;
    bbox_min.z = bbox_min.z > z ? z : bbox_min.z;

    bbox_max.x = bbox_max.x < io.pointlist[2*i  ] ? io.pointlist[2*i  ] : bbox_max.x;
    bbox_max.y = bbox_max.y < io.pointlist[2*i+1] ? io.pointlist[2*i+1] : bbox_max.y;
    bbox_max.z = bbox_max.z < z ? z : bbox_max.z;
  }

  RS_Vector bbox = bbox_max - bbox_min;

  int    NumContour = 20;
  double contLen = (bbox.z) / (NumContour + 1);

  for(int i=0; i<io.numberoftriangles; ++i)
  {

    // mesh triangle
    RS_Vector a(io.pointlist[2*io.trianglelist[3*i+0]], io.pointlist[2*io.trianglelist[3*i+0]+1]);
    RS_Vector b(io.pointlist[2*io.trianglelist[3*i+1]], io.pointlist[2*io.trianglelist[3*i+1]+1]);
    RS_Vector c(io.pointlist[2*io.trianglelist[3*i+2]], io.pointlist[2*io.trianglelist[3*i+2]+1]);

    this->addEntity(new RS_Line(this, RS_LineData(a,b)));
    this->addEntity(new RS_Line(this, RS_LineData(b,c)));
    this->addEntity(new RS_Line(this, RS_LineData(c,a)));

    //contour

    if(fabs(contLen)>1e-14)
    {
      a.z = _pm->profile(a.x, a.y);
      b.z = _pm->profile(b.x, b.y);
      c.z = _pm->profile(c.x, c.y);

      // a
      // |\
      // | \
      // b--c

      // Array to hold the points.
      RS_Vector temp1, temp2;
      RS_Vector tempa = a;
      RS_Vector tempb = b;
      RS_Vector tempc = c;

      // Sort the three points tempa, tempb, tempc in increasing z components;
      if (tempa.z > tempb.z) std::swap(tempa, tempb);
      if (tempb.z > tempc.z)
      {
        if(tempc.z < tempa.z) std::swap(tempa, tempc);
        std::swap(tempb, tempc);
      }

      // Loop though all the levels
      for (double level = bbox_min.z; level <= bbox_max.z; level += contLen)
      {
        // Check if the step size is in range: 1 < level < MaxContour

        if ((level == tempa.z) && (level == tempc.z))
        {
          this->addEntity(new RS_Line(this, RS_LineData(tempa, tempb)));
          this->addEntity(new RS_Line(this, RS_LineData(tempb, tempc)));
          this->addEntity(new RS_Line(this, RS_LineData(tempc, tempa)));
          break;
        }
        else if ((level >= tempa.z) && (level <= tempc.z))
        { // OK, test each edge, please.
          if((level == tempa.z) && (tempa.z == tempb.z) && (tempb.z != tempc.z))
          {
            temp1  = tempa;
            temp2  = tempb;
          }
          else if ((level == tempc.z) && (tempc.z == tempb.z) && (tempa.z != tempb.z))
          {
            temp1 = tempb;
            temp2 = tempc;
          }
          else if (level == tempb.z)
          { // Now tempb (level) MUST be between tempa and tempc
            // so a<level=b<c
            temp1 = linear_interpolation(tempa, tempc, level);
            temp2 = tempb;
          }
          else if (level < tempb.z)
          { // tempb and tempc above the tempa
            // a<level<b<c
            temp1 = linear_interpolation(tempa, tempb, level);
            temp2 = linear_interpolation(tempa, tempc, level);
          }
          else
          { // tempa and tempb are below tempc
            // a<b<level<c
            temp1 = linear_interpolation(tempa, tempc, level);
            temp2 = linear_interpolation(tempb, tempc, level);
          }

          this->addEntity(new RS_Line(this, RS_LineData(temp1, temp2)));
        }
      }
    }
  }
}


void RS_Mesh::draw(RS_Painter* painter, RS_GraphicView* view,  double /*patternOffset*/)
{

  if (painter==NULL || view==NULL)
  {
    return;
  }

  for (RS_Entity* e=firstEntity(RS2::ResolveNone); e!=NULL; e = nextEntity(RS2::ResolveNone))
  {
    view->drawEntity(e);
  }
}


RS_Vector RS_Mesh::linear_interpolation(RS_Vector a, RS_Vector b, double level)
{

  if(a.z > b.z ) std::swap(a, b);

  do
  {
    RS_Vector mid = (a+b)/2;
    mid.z = _pm->profile(mid.x, mid.y);
    if(mid.z < level)
      a = mid;
    else
      b = mid;
  }while ( sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y)) > 1e-4);

  return a;
}


void RS_Mesh::export_mesh(const RS_String & file)
{
  if(!io.numberofpoints || !io.numberoftriangles) return;

  QString def_unit = "Micron";
#ifdef QC_PREDEFINED_UNIT
  def_unit = QC_PREDEFINED_UNIT;
#endif
  RS_SETTINGS->beginGroup("/Defaults");
  QString unit_string = RS_SETTINGS->readEntry("/Unit", def_unit);
  RS_SETTINGS->endGroup();

  double mm = RS_Units::getFactorToMM( RS_Units::stringToUnit(unit_string) );
  double um = 1e3*mm;

  std::ofstream fout;
  fout.open(file.ascii(), std::ofstream::trunc);
  // set the float number precision
  fout.precision(8);

  // set output width and format
  fout<< std::scientific << std::right;

  time_t          _time;
  time(&_time);

  fout << "h TIF V1.2.1 created by QDRAW, Copyright (C) by Cogenda Inc. Date: " << ctime(&_time)  << '\n';
  fout << "cd GEN          blnk               blnk          blnk        cart2D    1.00000E+00  0.00000E+00"  << '\n';
  fout << "cg   3.00000E+02" << '\n';

  //write point
  for(int i=0; i<io.numberofpoints; ++i)
    fout<< 'c' << '\t'
    << i+1 << '\t'
    << io.pointlist[2*i+0]*um << '\t'
    << io.pointlist[2*i+1]*um << '\t'
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

  // multimap<point index, region index>
  std::multimap<int, int> point_region_map;
  typedef std::multimap<int, int>::iterator PM_It;
  for(int i=0; i<3*io.numberoftriangles; ++i)
  {
    int r = int(io.triangleattributelist[i/3]+0.5); // region index
    int p = io.trianglelist[i]; //point index
    std::pair<PM_It, PM_It> pm_it_pair = point_region_map.equal_range(p);

    std::set<int> region_list;
    PM_It pm_it = pm_it_pair.first;
    for(; pm_it!=pm_it_pair.second; ++pm_it)
      region_list.insert(pm_it->second);

    if(region_list.find(r) == region_list.end() )
      point_region_map.insert(std::make_pair(p,r));
  }
  // write profile information
  fout<< "s    6 Net Total Donor Accept N-type P-type" << '\n';

  PM_It pm_it = point_region_map.begin();
  for(;pm_it!=point_region_map.end(); ++pm_it)
  {
    double x = io.pointlist[2*pm_it->first+0];
    double y = io.pointlist[2*pm_it->first+1];
    double Na = fabs(_pm->profile("Na", x, y));
    double Nd = fabs(_pm->profile("Nd", x, y));

    fout<< 'n' << '\t'
    << pm_it->first+1 << '\t'
    << _region_mark_to_label_material[pm_it->second].second << '\t'
    << 0.5*(Nd-Na) << '\t'
    << 0.5*(Nd+Na) << '\t'
    << Nd << '\t'
    << Na << '\t'
    << Nd << '\t'
    << Na << '\n';
  }

  fout.close();
}
