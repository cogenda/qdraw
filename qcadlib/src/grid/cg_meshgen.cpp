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
#include <fstream>


#include "rs_string.h"
#include "rs_graphic.h"
#include "rs_graphicview.h"
#include "rs_spline.h"
#include "rs_hatch.h"
#include "rs_mesh.h"
#include "cg_meshgen.h"


MeshGenerator::MeshGenerator(RS_Document * doc, RS_GraphicView * gv)
    :_doc(doc), _gv(gv)
{
  triangulateio_init();
}


void MeshGenerator::convert_cad_to_pslg()
{
  RS_Graphic * g = _gv->getGraphic();
  if(g==NULL) return;

  //mesh all the entity on active layer
  for (RS_Entity* e=g->firstEntity(); e!=NULL; e=g->nextEntity())
    if(e->isOnActiveLayer() && e->isVisible())
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
          int mark;
          if(_label_to_mark.find(line->getLabel())!=_label_to_mark.end())
            mark = _label_to_mark[line->getLabel()];
          else
          {
            mark = _label_to_mark.size()+1;
            _label_to_mark[line->getLabel()] = mark;
          }

          unsigned int division = line->getDivision();
          RS_Vector  start = line->getStartpoint();
          RS_Vector  end = line->getEndpoint();
          RS_Vector  p1 = start;
          RS_Vector  p2 = p1 + (end - start)/division;

          for(unsigned int n=0; n<division; ++n)
          {
            if(!line->isPointSet())
            {
              CG_Segment segment;
              segment.p1 = add_point(p1);
              segment.p2 = add_point(p2);
              segment.mark = mark;
              _segments.push_back(segment);
            }
            else
            {
              add_point(p1);
              add_point(p2);
            }
            p1 = p2;
            p2 = p1 + (end - start)/division;
          }
          break;
        }

      case RS2::EntityArc:
        {
          const RS_Arc * arc = (const RS_Arc *)e;
          int mark;
          if(_label_to_mark.find(arc->getLabel())!=_label_to_mark.end())
            mark = _label_to_mark[arc->getLabel()];
          else
          {
            mark = _label_to_mark.size()+1;
            _label_to_mark[arc->getLabel()] = mark;
          }

          unsigned int division = arc->getDivision();
          RS_Vector  center = arc->getCenter();
          double     r = arc->getRadius();
          double     a1, a2;
          if(arc->isReversed())
          {
            // Arc Clockwise:
            a2 = arc->getAngle1();
            a1 = arc->getAngle2();
          }
          else
          {
            // Arc Counterclockwise:
            a1 = arc->getAngle1();
            a2 = arc->getAngle2();
          }
          if (a1>a2-0.01)  a2+=2*M_PI;

          RS_Vector  p1 = center + RS_Vector(r*cos(a1), r*sin(a1));
          RS_Vector  p2 = center + RS_Vector(r*cos(a1+(a2-a1)/division), r*sin(a1+(a2-a1)/division));
          for(unsigned int n=1; n<=division; ++n)
          {
            CG_Segment segment;
            segment.p1 = add_point(p1);
            segment.p2 = add_point(p2);
            segment.mark = mark;
            _segments.push_back(segment);
            p1 = p2;
            p2 = center + RS_Vector(r*cos(a1+(n+1)*(a2-a1)/division), r*sin(a1+(n+1)*(a2-a1)/division));
          }

          break;
        }

      case RS2::EntityCircle:
        {
          const RS_Circle * circle = (const RS_Circle *)e;
          int mark;
          if(_label_to_mark.find(circle->getLabel())!=_label_to_mark.end())
            mark = _label_to_mark[circle->getLabel()];
          else
          {
            mark = _label_to_mark.size()+1;
            _label_to_mark[circle->getLabel()] = mark;
          }

          unsigned int division = circle->getDivision();
          RS_Vector  center = circle->getCenter();
          RS_Vector  start = circle->getStartpoint();
          double     r = circle->getRadius();
          RS_Vector  p1 = start;
          RS_Vector  p2 = center + RS_Vector(r*cos(2*M_PI/division), r*sin(2*M_PI/division));
          for(unsigned int n=1; n<=division; ++n)
          {
            CG_Segment segment;
            segment.p1 = add_point(p1);
            segment.p2 = add_point(p2);
            segment.mark = mark;
            _segments.push_back(segment);
            p1 = p2;
            p2 = center + RS_Vector(r*cos((n+1)*2*M_PI/division), r*sin((n+1)*2*M_PI/division));
            if(n==division) p2 = start;
          }
          break;
        }

      case RS2::EntityEllipse :
        {
          const RS_Ellipse * ellipse = (const RS_Ellipse *)e;
          int mark;
          if(_label_to_mark.find(ellipse->getLabel())!=_label_to_mark.end())
            mark = _label_to_mark[ellipse->getLabel()];
          else
          {
            mark = _label_to_mark.size()+1;
            _label_to_mark[ellipse->getLabel()] = mark;
          }

          unsigned int division = ellipse->getDivision();
          RS_Vector  center = ellipse->getCenter();
          double     a = ellipse->getMajorRadius();
          double     b = ellipse->getMinorRadius();
          double     angel = ellipse->getAngle1();
          RS_Vector  start = center + RS_Vector(a*cos(angel), b*sin(angel));
          start.rotate(center, ellipse->getAngle());

          RS_Vector  p1 = start;
          RS_Vector  p2 = center + RS_Vector(a*cos(angel+2*M_PI/division), b*sin(angel+2*M_PI/division));
          p2.rotate(center, ellipse->getAngle());

          for(unsigned int n=1; n<=division; ++n)
          {
            CG_Segment segment;
            segment.p1 = add_point(p1);
            segment.p2 = add_point(p2);
            segment.mark = mark;
            _segments.push_back(segment);
            p1 = p2;
            p2 = center + RS_Vector(a*cos(angel+(n+1)*2*M_PI/division), b*sin(angel+(n+1)*2*M_PI/division));
            p2.rotate(center, ellipse->getAngle());
            if(n==division) p2 = start;
          }
          break;
        }

      case RS2::EntitySpline :
        {
          RS_Spline * spline = (RS_Spline *)e;
          int mark;
          if(_label_to_mark.find(spline->getLabel())!=_label_to_mark.end())
            mark = _label_to_mark[spline->getLabel()];
          else
          {
            mark = _label_to_mark.size()+1;
            _label_to_mark[spline->getLabel()] = mark;
          }

          for (RS_Entity* l=spline->firstEntity(); l!=NULL; l=spline->nextEntity())
            if(l->rtti() == RS2::EntityLine )
            {
              const RS_Line * line = (const RS_Line *)l;
              CG_Segment segment;
              segment.p1 = add_point(line->getStartpoint());
              segment.p2 = add_point(line->getEndpoint());
              segment.mark = mark;
              _segments.push_back(segment);
            }

          break;
        }

      case RS2::EntityHatch :
        {
          const RS_Hatch * hatch = (const RS_Hatch *)e;
          RS_HatchData data = hatch->getData();

          if(data.hole)
          {
            CG_Hole hole;
            hole.x    = data.internal_point.x;
            hole.y    = data.internal_point.y;
            _holes.push_back(hole);
          }
          else
          {
            CG_Region region;
            region.x    = data.internal_point.x;
            region.y    = data.internal_point.y;
            region.area_control = data.area_control > 0? data.area_control : RS_MAXDOUBLE;
            region.label = data.label;
            region.material = data.material;
            _regions.push_back(region);
          }
          break;
        }

      default: break;
      }
    }
}


MeshGenerator::~MeshGenerator()
{
  triangulateio_finalize();
}


unsigned int MeshGenerator::add_point(const RS_Vector &v)
{
  for(unsigned int n=0; n<_points.size(); ++n)
    if( v.distanceTo(_points[n])<1e-4 ) return n;
  _points.push_back(v);
  return _points.size()-1;
}


void MeshGenerator::do_mesh(const QString &cmd )
{
  triangulateio_init();

  // we only mesh cad entity on current layer, and visitable
  convert_cad_to_pslg();

  if(_points.size() < 3) return;

  //set point
  in.numberofpoints = _points.size();
  in.numberofpointattributes = 0;
  in.pointattributelist = (double *)NULL;
  in.pointlist = (double *) calloc(in.numberofpoints*2, sizeof(double));
  in.pointmarkerlist = (int *) calloc(in.numberofpoints, sizeof(int));

  double *ppointlist=in.pointlist;
  int *ppointmarkerlist=in.pointmarkerlist;
  //the points belongs to rectangle region
  for(unsigned int i=0; i<_points.size(); i++)
  {
    *ppointlist++ = _points[i].x;
    *ppointlist++ = _points[i].y;
    *ppointmarkerlist++ = 0;
  }

  //do necessarily prepare for call triangulate
  in.numberoftriangles = 0;
  in.numberofcorners = 3;
  in.numberoftriangleattributes = 0;
  in.trianglelist =  (int *) NULL;
  in.trianglearealist = (double *) NULL;
  in.triangleattributelist = NULL;

  // set segment information
  in.numberofsegments = _segments.size();

  in.segmentlist =  (int *) calloc(in.numberofsegments*2,  sizeof(int));
  in.segmentmarkerlist = (int *) calloc(in.numberofsegments, sizeof(int));

  int *psegmentlist =  in.segmentlist;
  int *psegmentmarkerlist = in.segmentmarkerlist;
  for(unsigned int i=0;i<_segments.size();i++)
  {
    *psegmentlist++ = _segments[i].p1;
    *psegmentlist++ = _segments[i].p2;
    *psegmentmarkerlist++ = _segments[i].mark;
  }

  //set region/hole information
  in.numberofregions = _regions.size();
  in.regionlist = (double *) calloc(in.numberofregions*4, sizeof(double));
  double *pregionlist = in.regionlist;
  for(unsigned int i=0;i<_regions.size();i++)
  {
    *pregionlist++ = _regions[i].x;
    *pregionlist++ = _regions[i].y;
    *pregionlist++ = double(i);
    *pregionlist++ = _regions[i].area_control;
  }

  in.numberofholes = _holes.size();
  in.holelist   = (double *) calloc(in.numberofholes*2, sizeof(double));
  double *pholelist = in.holelist;
  for(unsigned int i=0;i<_holes.size();i++)
  {
    *pholelist++ = _holes[i].x;
    *pholelist++ = _holes[i].y;
  }

  // call Triangle here
  triangulate(cmd.ascii(), &in, &out, (struct triangulateio *) NULL);

  RS_Mesh* mesh = draw_mesh();

  triangulateio_copy(out, mesh->get_triangulateio());
  for(std::map<RS_String, int>::iterator it=_label_to_mark.begin(); it!=_label_to_mark.end(); ++it)
    mesh->add_segment_info(it->second, it->first);
  for(unsigned int i=0;i<_regions.size();i++)
    mesh->add_region_info(i, _regions[i].label, _regions[i].material);
  mesh->tri_cmd() = cmd;

  triangulateio_finalize();
}


void MeshGenerator::refine_mesh(const QString &cmd)
{
  triangulateio_init();

  // find existing RS_Mesh oject
  RS_Graphic * g = _gv->getGraphic();
  if(g==NULL) return;

  for (RS_Entity* e=g->firstEntity(); e!=NULL; e=g->nextEntity())
    if(e->isOnActiveLayer() && e->isVisible() && e->rtti()== RS2::EntityMesh)
    {}

  // modify the mesh , do not create new mesh!


  triangulateio_finalize();
}



RS_Mesh* MeshGenerator::draw_mesh()
{
  //create new layer
  RS_Graphic * graphic = _gv->getGraphic();

  QString layer_name = "mesh";
  int i = 2;

  if (graphic->getLayerList()!=NULL)
  {
    while (graphic->getLayerList()->find(layer_name) > 0)
      layer_name.sprintf("mesh%d", i++);
  }

  RS_Layer* layer = new RS_Layer(layer_name);

  graphic->addLayer(layer); //the new layer is set to active automatically

  RS_Mesh* mesh = new RS_Mesh(_doc);

  for(int i=0; i<out.numberoftriangles; ++i)
  {
    RS_Vector p1(out.pointlist[2*out.trianglelist[3*i+0]],out.pointlist[2*out.trianglelist[3*i+0]+1]);
    RS_Vector p2(out.pointlist[2*out.trianglelist[3*i+1]],out.pointlist[2*out.trianglelist[3*i+1]+1]);
    RS_Vector p3(out.pointlist[2*out.trianglelist[3*i+2]],out.pointlist[2*out.trianglelist[3*i+2]+1]);

    mesh->addEntity(new RS_Line(mesh, RS_LineData(p1,p2)));
    mesh->addEntity(new RS_Line(mesh, RS_LineData(p2,p3)));
    mesh->addEntity(new RS_Line(mesh, RS_LineData(p3,p1)));
  }

  _doc->addEntity(mesh);
  _gv->drawEntity(mesh);

  return mesh;
}




void MeshGenerator::triangulateio_init()
{
  // init Triangle io structure.
  in.pointlist = (double *) NULL;
  in.pointattributelist = (double *) NULL;
  in.pointmarkerlist = (int *) NULL;
  in.trianglearealist= (double *) NULL;
  in.segmentlist = (int *) NULL;
  in.segmentmarkerlist = (int *) NULL;
  in.regionlist = (double *)NULL;
  in.holelist = (double *)NULL;

  out.numberofpoints = 0;
  out.pointlist = (double *) NULL;
  out.pointattributelist = (double *) NULL;
  out.pointmarkerlist = (int *) NULL;
  out.numberoftriangles = 0;
  out.trianglelist = (int *) NULL;
  out.triangleattributelist = (double *) NULL;
  out.trianglearealist = (double *) NULL;
  out.numberofsegments = 0;
  out.segmentlist = (int *) NULL;
  out.segmentmarkerlist = (int *) NULL;
  out.numberofregions = 0;
  out.numberofholes = 0;
}


void MeshGenerator::triangulateio_finalize()
{
  free(in.pointlist);
  in.pointlist=0;
  free(in.pointmarkerlist);
  in.pointmarkerlist=0;
  free(in.pointattributelist);
  in.pointattributelist=0;

  free(in.segmentlist);
  in.segmentlist=0;
  free(in.segmentmarkerlist);
  in.segmentmarkerlist=0;

  free(in.trianglelist);
  free(in.triangleattributelist);
  free(in.trianglearealist);
  in.numberoftriangles=0;

  free(in.regionlist);
  in.regionlist=0;
  free(in.holelist);
  in.holelist=0;

  out.numberofpoints = 0;
  free(out.pointlist);
  out.pointlist=0;
  free(out.pointmarkerlist);
  out.pointmarkerlist=0;
  free(out.pointattributelist);
  out.pointattributelist=0;

  out.numberoftriangles = 0;
  free(out.trianglelist);
  out.trianglelist=0;
  free(out.triangleattributelist);
  out.triangleattributelist=0;

  out.numberofsegments = 0;
  free(out.segmentlist);
  out.segmentlist=0;
  free(out.segmentmarkerlist);
  out.segmentmarkerlist=0;
}



void MeshGenerator::triangulateio_copy(const triangulateio & src, triangulateio & dst)
{
  dst.numberofpoints = src.numberofpoints;
  dst.numberofpointattributes = src.numberofpointattributes;

  if(dst.numberofpoints)
  {
    dst.pointlist = (double *) calloc(dst.numberofpoints*2, sizeof(double));
    dst.pointmarkerlist = (int *) calloc(dst.numberofpoints, sizeof(int));

    for(int i=0; i<dst.numberofpoints; ++i)
    {
      dst.pointlist[2*i+0]   = src.pointlist[2*i+0];
      dst.pointlist[2*i+1]   = src.pointlist[2*i+1];
      dst.pointmarkerlist[i] = src.pointmarkerlist[i];
    }
  }

  if(dst.numberofpointattributes)
  {
    dst.pointattributelist = (double *)calloc(dst.numberofpoints*dst.numberofpointattributes, sizeof(double));
    for(int i=0; i<dst.numberofpoints; ++i)
      for(int j=0; j<dst.numberofpointattributes;++j)
        dst.pointattributelist[dst.numberofpointattributes*i+j] = \
            src.pointattributelist[dst.numberofpointattributes*i+j];
  }


  dst.numberoftriangles = src.numberoftriangles;
  if(dst.numberoftriangles)
  {
    dst.trianglelist = (int *) calloc(3*dst.numberoftriangles, sizeof(int));
    for(int i=0; i<dst.numberoftriangles; ++i)
    {
      dst.trianglelist[3*i+0]   = src.trianglelist[3*i+0];
      dst.trianglelist[3*i+1]   = src.trianglelist[3*i+1];
      dst.trianglelist[3*i+2]   = src.trianglelist[3*i+2];
    }
  }

  dst.numberoftriangleattributes = src.numberoftriangleattributes;
  if(dst.numberoftriangleattributes)
  {
    dst.triangleattributelist=(double *) calloc(dst.numberoftriangleattributes*dst.numberoftriangles, sizeof(double));
    for(int i=0; i<dst.numberoftriangles; ++i)
      for(int j=0; j<dst.numberoftriangleattributes;++j)
        dst.triangleattributelist[dst.numberoftriangleattributes*i+j] = \
            src.triangleattributelist[dst.numberoftriangleattributes*i+j];
  }
  else
  {
    dst.triangleattributelist=(double *) calloc(dst.numberoftriangles, sizeof(double));
    for(int i=0; i<dst.numberoftriangles; ++i)
      dst.triangleattributelist[i] = 0;
  }

  dst.numberofsegments = src.numberofsegments;
  if(dst.numberofsegments)
  {
    dst.segmentlist =  (int *) calloc(dst.numberofsegments*2,  sizeof(int));
    dst.segmentmarkerlist = (int *) calloc(dst.numberofsegments, sizeof(int));
    for(int i=0; i<dst.numberofsegments; ++i)
    {
      dst.segmentlist[2*i+0]   = src.segmentlist[2*i+0];
      dst.segmentlist[2*i+1]   = src.segmentlist[2*i+1];
      dst.segmentmarkerlist[i] = src.segmentmarkerlist[i];
    }
  }

  dst.numberofholes = src.numberofholes;
  if(dst.numberofholes)
  {
    dst.holelist   = (double *) calloc(dst.numberofholes*2, sizeof(double));
    for(int i=0; i<dst.numberofholes; ++i)
    {
      dst.holelist[2*i+0]   = src.holelist[2*i+0];
      dst.holelist[2*i+1]   = src.holelist[2*i+1];
    }
  }

  dst.numberofregions = src.numberofregions;
  if(dst.numberofregions)
  {
    dst.regionlist   = (double *) calloc(dst.numberofregions*2, sizeof(double));
    for(int i=0; i<dst.numberofregions; ++i)
    {
      dst.regionlist[2*i+0]   = src.regionlist[2*i+0];
      dst.regionlist[2*i+1]   = src.regionlist[2*i+1];
    }
  }

}


void MeshGenerator::export_mesh_vtk(const char * name)
{

  std::ofstream fout;
  fout.open(name, std::ofstream::trunc);

  fout << "# vtk DataFile Version 3.0" <<'\n';
  fout << "Date calculated by CTRI"    <<'\n';
  fout << "ASCII"                      <<'\n';
  fout << "DATASET UNSTRUCTURED_GRID"  <<'\n';
  fout << "POINTS " << out.numberofpoints  << " float" << '\n';

  for (int i=0; i<out.numberofpoints; ++i)
  {
    fout << out.pointlist[2*i+0] << " \t" << out.pointlist[2*i+1] << " \t " << 0.0 << '\n';
  }

  fout << std::endl;

  fout<<"CELLS "<<out.numberoftriangles<<" "<<4*out.numberoftriangles<<'\n';

  for(int i=0; i<out.numberoftriangles; ++i)
  {
    fout << 3 << " "
    << out.trianglelist[3*i+0] << " "
    << out.trianglelist[3*i+1] << " "
    << out.trianglelist[3*i+2] << "\n";
  }

  fout << std::endl;

  fout << "CELL_TYPES " << out.numberoftriangles << '\n';

  for(int i=0; i<out.numberoftriangles; ++i)
    fout << 5 << std::endl;

  fout << std::endl;

  if( out.triangleattributelist!= NULL)
  {
    fout << "CELL_DATA "<<out.numberoftriangles     <<'\n';
    fout << "SCALARS region float 1"  <<'\n';
    fout << "LOOKUP_TABLE default"    <<'\n';
    for(int i=0; i<out.numberoftriangles; ++i)
    {
      fout << static_cast<int>(out.triangleattributelist[i]+0.5)<<'\n';
    }
    fout<<std::endl;
  }


  /*
          //write node based boundary info to vtk
       fout<<"POINT_DATA "<< _points.size()      <<'\n';
       fout<<"SCALARS point_mark float 1" <<'\n';
       fout<<"LOOKUP_TABLE default"       <<'\n';
       for(unsigned int n=0; n<_points.size(); ++n)
       {
       	fout<<_points[n]->mark()<<'\n' ;
       }
       fout<<std::endl;
       */
  fout.close();


}
