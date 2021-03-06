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
#include <map>
#include <fstream>

#include <qmessagebox.h>

#include "rs_string.h"
#include "rs_graphic.h"
#include "rs_graphicview.h"
#include "rs_dialogfactory.h"
#include "rs_mesh.h"
#include "cg_meshgen.h"
#include "cg_quadtree.h"
#include "cg_profile_manager.h"

MeshGenerator::MeshGenerator(RS_Document * doc, RS_GraphicView * gv, ProfileManager *pm)
    :_doc(doc), _gv(gv), _pm(pm)
{
  triangulateio_init();
}


MeshGenerator::~MeshGenerator()
{
  triangulateio_finalize();
}



void MeshGenerator::do_mesh(const QString &cmd, bool enable_quadtree, bool fit_cv_boundary)
{
  triangulateio_init();

  //create PSLG
  RS_DIALOGFACTORY->commandMessage(("Create Planar Straight Line Graph (PSLG) by meshable entities on current layer..."));
  _pslg = new CG_PSLG(_gv->getGraphic());

  std::vector<RS_Vector> &  _points       = _pslg->get_points();
  std::vector<RS_Vector> &  _aux_points   = _pslg->get_aux_points();
  std::vector<CG_Segment> & _segments     = _pslg->get_segments();
  std::vector<CG_Region> &  _regions      = _pslg->get_regions();
  std::vector<CG_Hole> &    _holes        = _pslg->get_holes();

  if(_points.size() < 3)
  {
    QMessageBox::critical( 0, "Mesh Generation", "No Mesh generated due to invalid geometry");
    return;
  }

  QuadTree * quadtree = NULL;
  if(enable_quadtree)
  {
    RS_DIALOGFACTORY->commandMessage(("Create QuadTree on PSLG..."));
    quadtree = build_quadtree();
    //quadtree->export_quadtree("aa.vtk");
  }

  //set point
  in.numberofpoints = _points.size() + _aux_points.size();
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
  for(unsigned int i=0; i<_aux_points.size(); i++)
  {
    *ppointlist++ = _aux_points[i].x;
    *ppointlist++ = _aux_points[i].y;
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
  RS_DIALOGFACTORY->commandMessage(("Do Delaunay Triangulation..."));
  triangulate(cmd.ascii(), &in, &out, (struct triangulateio *) NULL);

  //try to fit curved boundary
  if(fit_cv_boundary)
    repair_mesh(out);

  RS_String point_num_to_string;
  RS_String triangle_num_to_string;
  point_num_to_string.setNum(out.numberofpoints);
  triangle_num_to_string.setNum(out.numberoftriangles);
  RS_String mesh_statistical = "Total " + point_num_to_string + " Points and " + triangle_num_to_string + " Triangles created.";
  RS_DIALOGFACTORY->commandMessage(mesh_statistical);

  RS_DIALOGFACTORY->commandMessage(("Draw mesh entity on new layer..."));
  create_new_mesh_layer();
  RS_Mesh* mesh =  new RS_Mesh(_doc);

  triangulateio_copy(out, mesh->get_triangulateio());
  mesh->set_pslg(_pslg);
  mesh->tri_cmd() = cmd;
  mesh->set_profile_manager(_pm);
  mesh->set_quadtree(quadtree);
  mesh->update();

  _doc->addEntity(mesh);
  _gv->drawEntity(mesh);

  triangulateio_finalize();

  RS_DIALOGFACTORY->commandMessage(("Mesh generation finished."));
}



QuadTree * MeshGenerator::build_quadtree()
{
  std::vector<RS_Vector> &  _points   = _pslg->get_points();
  std::vector<RS_Vector> &  _aux_points   = _pslg->get_aux_points();
  std::vector<CG_Constrain> & _constrains = _pslg->get_constrain();
  std::vector<CG_Region> & _regions = _pslg->get_regions();

  // create quad bound box
  RS_Vector bl(RS_MAXDOUBLE, RS_MAXDOUBLE), tr(-RS_MAXDOUBLE, -RS_MAXDOUBLE);
  for(unsigned int n=0; n<_points.size(); ++n)
  {
    bl.x = bl.x > _points[n].x ? _points[n].x : bl.x;
    bl.y = bl.y > _points[n].y ? _points[n].y : bl.y;

    tr.x = tr.x < _points[n].x ? _points[n].x : tr.x;
    tr.y = tr.y < _points[n].y ? _points[n].y : tr.y;
  }
  if(tr.x - bl.x > tr.y - bl.y)
    tr.y = bl.y + (tr.x - bl.x);
  else
    tr.x = bl.x + (tr.y - bl.y);

  RS_Vector br(tr.x, bl.y);
  RS_Vector tl(bl.x, tr.y);

  // create root quadtree
  QuadTree * quadtree = new QuadTree;

  const RS_Vector * p_bl = quadtree->add_point(bl);
  const RS_Vector * p_br = quadtree->add_point(br);
  const RS_Vector * p_tr = quadtree->add_point(tr);
  const RS_Vector * p_tl = quadtree->add_point(tl);
  quadtree->set_head(QuadTreeNodeData(p_tl, p_tr, p_br, p_bl, QuadTreeLocation()));

  // loop. until area constrain satisfied
  bool area_constrain;
  do
  {
    area_constrain=false;

    QuadTree::leaf_iterator leaf_it = quadtree->begin_leaf();
    for(; leaf_it != quadtree->end_leaf(); )
    {
      QuadTree::iterator this_leaf = leaf_it++;
      bool this_leaf_should_be_divide = false;

      // check line constrain
      if(this_leaf->line_intersection_flag()!=QuadTreeNodeData::NO_INTERSECTION)
      {
        bool line_intersection = false;
        for(unsigned int n=0; n<_constrains.size(); ++n)
        {
          if(quadtree->is_line_intersection(this_leaf, _constrains[n].p1, _constrains[n].p2 ))
          {
            line_intersection = true;
            this_leaf->line_intersection_flag()=QuadTreeNodeData::HAS_INTERSECTION;
            if(this_leaf->area()>_constrains[n].char_length*_constrains[n].char_length)
              this_leaf_should_be_divide = true;
          }
        }
        if(!line_intersection)
          this_leaf->line_intersection_flag()=QuadTreeNodeData::NO_INTERSECTION;
      }

      // check region constrain
      switch(this_leaf->region_intersection_flag())
      {
      case QuadTreeNodeData::IN_REGION  :
        if(this_leaf->area()>_regions[this_leaf->region()].area_control)
          this_leaf_should_be_divide = true;
        break;
      case QuadTreeNodeData::OUT_REGION :  break;
      case QuadTreeNodeData::COVER_REGION :
      case QuadTreeNodeData::INTERSECTION_REGION :
      case QuadTreeNodeData::REGION_INTERSECTION_UNKNOW :
        {
          //set initial flag to unknow
          this_leaf->region_intersection_flag() = QuadTreeNodeData::REGION_INTERSECTION_UNKNOW;
          for(unsigned int n=0; n<_regions.size(); ++n)
          {
            switch(quadtree->region_intersection(this_leaf, _regions[n]))
            {
            case QuadTreeNodeData::OUT_REGION :
              break;
            case QuadTreeNodeData::IN_REGION  :
              {
                this_leaf->region_intersection_flag() = QuadTreeNodeData::IN_REGION;
                this_leaf->region()=n;
                if(this_leaf->area()>_regions[n].area_control)
                  this_leaf_should_be_divide = true;
                break;
              }
            case QuadTreeNodeData::INTERSECTION_REGION :
              {
                this_leaf->region_intersection_flag() = QuadTreeNodeData::INTERSECTION_REGION;
                this_leaf->record_intersection_region(n);
                if(this_leaf->area()>_regions[n].area_control)
                  this_leaf_should_be_divide = true;
                break;
              }
            case QuadTreeNodeData::COVER_REGION  :
              {
                this_leaf->region_intersection_flag() = QuadTreeNodeData::COVER_REGION;
                this_leaf_should_be_divide = true;
                break;
              }
            default: break;
            }

            if(this_leaf->region_intersection_flag() == QuadTreeNodeData::IN_REGION) break;
            if(this_leaf->region_intersection_flag() == QuadTreeNodeData::COVER_REGION) break;
          }

          // if still REGION_INTERSECTION_UNKNOW, mark it to OUT_REGION
          if(this_leaf->region_intersection_flag() == QuadTreeNodeData::REGION_INTERSECTION_UNKNOW)
            this_leaf->region_intersection_flag() = QuadTreeNodeData::OUT_REGION;

        }
        break;
      }

      if(this_leaf_should_be_divide)
      {
        quadtree->subdivide(this_leaf);
        area_constrain = true;
      }
    }

    if(area_constrain)
      quadtree->balance();
  }
  while(area_constrain);

  // add quadtree mesh to PSLG points
  {
    _aux_points.clear();

    std::map<const RS_Vector *, double> quadtree_points;
    QuadTree::leaf_iterator leaf_it = quadtree->begin_leaf();
    for(; leaf_it != quadtree->end_leaf(); ++leaf_it)
      if(leaf_it->region_intersection_flag()==QuadTreeNodeData::IN_REGION)
      {
        if(quadtree->has_neighbor_intersection_region(leaf_it))
        {
          quadtree_points[leaf_it->tl()] = leaf_it->char_length();
          quadtree_points[leaf_it->tr()] = leaf_it->char_length();
          quadtree_points[leaf_it->br()] = leaf_it->char_length();
          quadtree_points[leaf_it->bl()] = leaf_it->char_length();
        }
        else
        {
          if(quadtree_points.find(leaf_it->tl())==quadtree_points.end())
            quadtree_points[leaf_it->tl()] = 0;
          if(quadtree_points.find(leaf_it->tr())==quadtree_points.end())
            quadtree_points[leaf_it->tr()] = 0;
          if(quadtree_points.find(leaf_it->br())==quadtree_points.end())
            quadtree_points[leaf_it->br()] = 0;
          if(quadtree_points.find(leaf_it->bl())==quadtree_points.end())
            quadtree_points[leaf_it->bl()] = 0;
        }
      }

    std::map<const RS_Vector *, double>::iterator it=quadtree_points.begin();
    for(; it!=quadtree_points.end(); ++it)
      _pslg->add_aux_point(*(*it).first, (*it).second/3.0);
  }

  return quadtree;
}



void MeshGenerator::refine_mesh(const QString &cmd, double max_d,
                                bool signed_log, bool enable_quadtree, bool fit_cv_boundary)
{
  triangulateio_init();

  // find existing RS_Mesh oject
  RS_Graphic * g = _gv->getGraphic();
  if(g==NULL) return;

  RS_Mesh* mesh = find_mesh();
  if(mesh==NULL) return;

  _pslg = mesh->get_pslg();

  // refine by quadtree
  if(enable_quadtree && mesh->get_quadtree())
  {
    RS_DIALOGFACTORY->commandMessage(("Refine existing mesh by QuadTree method..."));

    QuadTree * quadtree = mesh->get_quadtree();

    std::vector<RS_Vector> &  _points       = _pslg->get_points();
    std::vector<RS_Vector> &  _aux_points   = _pslg->get_aux_points();
    std::vector<CG_Segment> & _segments     = _pslg->get_segments();
    std::vector<CG_Region> &  _regions      = _pslg->get_regions();
    std::vector<CG_Hole> &    _holes        = _pslg->get_holes();

    // refine only once!
    {
      bool area_constrain=false;

      QuadTree::leaf_iterator leaf_it = quadtree->begin_leaf();
      for(; leaf_it != quadtree->end_leaf(); )
      {
        QuadTree::iterator this_leaf = leaf_it++;
        if( mesh->is_refine_required(this_leaf, max_d, signed_log) )
        {
          quadtree->subdivide(this_leaf);
          area_constrain = true;
        }
      }
      if(area_constrain) quadtree->balance();
    }

    // set region intersection flag to all new generated leaf which has INTERSECTION_REGION flag
    {
      QuadTree::leaf_iterator leaf_it = quadtree->begin_leaf();
      for(; leaf_it != quadtree->end_leaf(); ++leaf_it)
      {
        // check region constrain
        if(leaf_it->region_intersection_flag()==QuadTreeNodeData::INTERSECTION_REGION ||
            leaf_it->region_intersection_flag()==QuadTreeNodeData::REGION_INTERSECTION_UNKNOW)
        {

          leaf_it->region_intersection_flag()=QuadTreeNodeData::REGION_INTERSECTION_UNKNOW;

          for(unsigned int n=0; n<_regions.size(); ++n)
          {
            switch(quadtree->region_intersection(leaf_it, _regions[n]))
            {
            case QuadTreeNodeData::OUT_REGION :
              {
                break;
              }
            case QuadTreeNodeData::IN_REGION  :
              {
                leaf_it->region_intersection_flag() = QuadTreeNodeData::IN_REGION;
                leaf_it->region()=n;
                break;
              }
            case QuadTreeNodeData::INTERSECTION_REGION :
              {
                leaf_it->region_intersection_flag() = QuadTreeNodeData::INTERSECTION_REGION;
                leaf_it->record_intersection_region(n);
                break;
              }
            default: break;
            }

            if(leaf_it->region_intersection_flag() == QuadTreeNodeData::IN_REGION) break;
          }

          // if still REGION_INTERSECTION_UNKNOW, mark it to OUT_REGION
          if(leaf_it->region_intersection_flag() == QuadTreeNodeData::REGION_INTERSECTION_UNKNOW)
            leaf_it->region_intersection_flag() = QuadTreeNodeData::OUT_REGION;
        }
      }

    }

    // add quadtree mesh to PSLG points
    {
      _aux_points.clear();

      std::map<const RS_Vector *, double> quadtree_points;
      QuadTree::leaf_iterator leaf_it = quadtree->begin_leaf();
      for(; leaf_it != quadtree->end_leaf(); ++leaf_it)
      {
        if(quadtree->has_neighbor_intersection_region(leaf_it))
        {
          quadtree_points[leaf_it->tl()] = leaf_it->char_length();
          quadtree_points[leaf_it->tr()] = leaf_it->char_length();
          quadtree_points[leaf_it->br()] = leaf_it->char_length();
          quadtree_points[leaf_it->bl()] = leaf_it->char_length();
        }
        else
        {
          if(quadtree_points.find(leaf_it->tl())==quadtree_points.end())
            quadtree_points[leaf_it->tl()] = 0;
          if(quadtree_points.find(leaf_it->tr())==quadtree_points.end())
            quadtree_points[leaf_it->tr()] = 0;
          if(quadtree_points.find(leaf_it->br())==quadtree_points.end())
            quadtree_points[leaf_it->br()] = 0;
          if(quadtree_points.find(leaf_it->bl())==quadtree_points.end())
            quadtree_points[leaf_it->bl()] = 0;
        }
      }
      std::map<const RS_Vector *, double>::iterator it=quadtree_points.begin();
      for(; it!=quadtree_points.end(); ++it)
        _pslg->add_aux_point(*(*it).first, (*it).second/3.0);
    }

    //set point
    in.numberofpoints = _points.size() + _aux_points.size();
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
    for(unsigned int i=0; i<_aux_points.size(); i++)
    {
      *ppointlist++ = _aux_points[i].x;
      *ppointlist++ = _aux_points[i].y;
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
    // replace 'r' to 'A'
    QString tri_cmd = cmd;
    tri_cmd.replace(tri_cmd.find('r'),1,'A');

    RS_DIALOGFACTORY->commandMessage(("Create refined mesh..."));
    triangulate(tri_cmd.ascii(), &in, &out, (struct triangulateio *) NULL);
  }
  else // refine by delaunay
  {
    RS_DIALOGFACTORY->commandMessage(("Refine existing mesh by Delaunay method..."));
    mesh->set_refine_flag(max_d, signed_log);
    triangulateio & mesh_in = mesh->get_triangulateio();
    triangulate(cmd.ascii(), &mesh_in, &out, (struct triangulateio *) NULL);
  }

  //try to fit curved boundary
  if(fit_cv_boundary)
    repair_mesh(out);

  _gv->deleteEntity(mesh);

  mesh->clear();
  triangulateio_copy(out, mesh->get_triangulateio());

  RS_DIALOGFACTORY->commandMessage(("Draw mesh entity..."));
  mesh->update();
  _gv->drawEntity(mesh);

  triangulateio_finalize();

  RS_DIALOGFACTORY->commandMessage(("Mesh refinement finished."));
}



void MeshGenerator::repair_mesh(triangulateio &t)
{
  for(int i=0; i<t.numberofsegments; ++i)
  {
    int mark = t.segmentmarkerlist[i];
    int ep1 = t.segmentlist[2*i];
    int ep2 = t.segmentlist[2*i+1];
    RS_Vector p1(t.pointlist[2*ep1], t.pointlist[2*ep1+1]);
    RS_Vector p2(t.pointlist[2*ep2], t.pointlist[2*ep2+1]);
    std::vector<RS_Entity *> entities = _pslg->get_entities_by_mark(mark);
    for(unsigned int n=0; n<entities.size(); ++n)
    {
      RS_Entity * e = entities[n];
      switch (e->rtti())
      {
      case RS2::EntityCircle :
        {
          RS_Circle * circle = (RS_Circle *)e;
          double dist1, dist2;
          RS_Vector mp1 = circle->getNearestPointOnEntity(p1, true, &dist1);
          RS_Vector mp2 = circle->getNearestPointOnEntity(p2, true, &dist2);
          //move p1, p2 to on entity point mp1, mp2
          if(dist1<0.5*(p2-p1).magnitude() && dist2<0.5*(p2-p1).magnitude())
          {
            p1 = mp1;
            p2 = mp2;
          }
        }
        break;
      case RS2::EntityEllipse :
        {
          RS_Ellipse * ellipse = (RS_Ellipse *)e;
          double dist1, dist2;
          RS_Vector mp1 = ellipse->getNearestPointOnEntity(p1, true, &dist1);
          RS_Vector mp2 = ellipse->getNearestPointOnEntity(p2, true, &dist2);
          //move p1, p2 to on entity point mp1, mp2
          if(dist1<0.5*(p2-p1).magnitude() && dist2<0.5*(p2-p1).magnitude())
          {
            p1 = mp1;
            p2 = mp2;
          }
        }
        break;
      case RS2::EntityArc:
        {
          RS_Arc * arc = (RS_Arc *)e;
          double dist1, dist2;
          RS_Vector mp1 = arc->getNearestPointOnEntity(p1, true, &dist1);
          RS_Vector mp2 = arc->getNearestPointOnEntity(p2, true, &dist2);
          //move p1, p2 to on entity point mp1, mp2
          if(dist1<0.5*(p2-p1).magnitude() && dist2<0.5*(p2-p1).magnitude())
          {
            p1 = mp1;
            p2 = mp2;
          }
        }
        break;
      case RS2::EntitySpline :
        {
          RS_Spline * spline = (RS_Spline *)e;
          double dist1, dist2;
          RS_Vector mp1 = spline->getNearestPointOnEntity(p1, true, &dist1);
          RS_Vector mp2 = spline->getNearestPointOnEntity(p2, true, &dist2);
          //move p1, p2 to on entity point mp1, mp2
          if(dist1<0.5*(p2-p1).magnitude() && dist2<0.5*(p2-p1).magnitude())
          {
            p1 = mp1;
            p2 = mp2;
          }
        }
        break;
      default: break;
      }
    }

    //update mesh point location
    t.pointlist[2*ep1] = p1.x;
    t.pointlist[2*ep1+1]=p1.y;
    t.pointlist[2*ep2]=p2.x;
    t.pointlist[2*ep2+1]=p2.y;
  }


  //check if all the boundary points on entities
  bool all_on_entity=true;
  for(int i=0; i<t.numberofsegments; ++i)
  {
    int mark = t.segmentmarkerlist[i];
    int ep1 = t.segmentlist[2*i];
    int ep2 = t.segmentlist[2*i+1];
    bool this_edge_on_entity=false;
    RS_Vector p1(t.pointlist[2*ep1], t.pointlist[2*ep1+1]);
    RS_Vector p2(t.pointlist[2*ep2], t.pointlist[2*ep2+1]);
    std::vector<RS_Entity *> entities = _pslg->get_entities_by_mark(mark);
    for(unsigned int n=0; n<entities.size(); ++n)
    {
      double dist1, dist2;
      RS_Vector mp1 = entities[n]->getNearestPointOnEntity(p1, true, &dist1);
      RS_Vector mp2 = entities[n]->getNearestPointOnEntity(p2, true, &dist2);
      if(dist1<1e-6*(p2-p1).magnitude() && dist2<1e-6*(p2-p1).magnitude())
      {
        this_edge_on_entity = true;
        break;
      }
    }
    all_on_entity &= this_edge_on_entity;
  }

  if(!all_on_entity)
    QMessageBox::warning(NULL, QObject::tr("Curved Boundary Fit"),
                         QObject::tr("Still some boundary edges lie on the approximate "
                                     "polygon instead of curved boundary. <br>"
                                     "This usually won't affect later numerical simulation. <br>"
                                     "However, to get a perfet matched curved boundary, "
                                     "you may try to divide curved entities into more "
                                     "initial segments, and mesh again.<br>"
                                    )
                        );

}



void MeshGenerator::create_new_mesh_layer()
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

}



RS_Mesh* MeshGenerator::find_mesh()
{
  //find if mesh exist
  RS_Mesh * mesh = NULL;
  RS_Mesh * mesh_select = NULL;
  unsigned int n_mesh = 0;
  unsigned int n_mesh_select = 0;

  RS_Graphic * g = _gv->getGraphic();
  if(g==NULL) return NULL;

  //mesh all the entity on active layer
  for (RS_Entity* e=g->firstEntity(); e!=NULL; e=g->nextEntity())
    if(e->isOnActiveLayer() && e->isVisible() && e->rtti() == RS2::EntityMesh)
    {
      mesh = (RS_Mesh *)e;
      n_mesh++;
      if(e->isSelected())
      {
        mesh_select = (RS_Mesh *)e;
        n_mesh_select++;
      }
    }

  // only one mesh object exist
  if(mesh && n_mesh==1) return mesh;

  // return the select mesh
  if(mesh_select && n_mesh_select==1) return mesh_select;

  //errors
  if(mesh==NULL)
  {
    QMessageBox::critical( 0, "Mesh Refinement", "No mesh can be found");
    return NULL;
  }

  if(n_mesh_select>1)
  {
    QMessageBox::critical( 0, "Mesh Refinement", "More than one mesh select");
    return NULL;
  }

  return NULL;
}


void MeshGenerator::triangulateio_init()
{
  // init Triangle io structure.
  in.pointlist = (double *) NULL;
  in.pointattributelist = (double *) NULL;
  in.pointmarkerlist = (int *) NULL;
  in.trianglelist = (int *) NULL;
  in.trianglearealist = (double *) NULL;
  in.triangleattributelist = (double *) NULL;
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

  fout.close();


}
