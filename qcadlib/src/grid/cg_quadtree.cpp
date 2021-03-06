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
#include <stack>

#include  "poly.h"
#include  "nclip.h"
#include  "cg_pslg.h"
#include  "cg_quadtree.h"
#include  "rs_information.h"


std::ostream& operator << (std::ostream& os, const QuadTreeNodeData& data)
{
  data.get_location().print(os);
  os << " "<< data.area() <<std::endl;
  return os;
}


QuadTree::~QuadTree()
{
  for(unsigned int n=0; n<_points.size(); ++n)
    delete _points[n];
}



void QuadTree::subdivide(iterator_base & leaf_it)
{
  QuadTreeNodeData leaf_data = *leaf_it;
  const RS_Vector * _p_tr = leaf_data.tr();
  const RS_Vector * _p_tl = leaf_data.tl();
  const RS_Vector * _p_br = leaf_data.br();
  const RS_Vector * _p_bl = leaf_data.bl();

  RS_Vector center((*_p_tr + *_p_bl)*0.5);

  RS_Vector top((*_p_tr + *_p_tl)*0.5);   //mid point of top line
  RS_Vector bot((*_p_br + *_p_bl)*0.5);   //mid point of bot line
  RS_Vector left((*_p_tl + *_p_bl)*0.5);  //mid point of left line
  RS_Vector right((*_p_tr + *_p_br)*0.5); //mid point of righy line

  const RS_Vector * _p_center = this->add_point(center);
  const RS_Vector * _p_top   = this->add_point(top);
  const RS_Vector * _p_bot   = this->add_point(bot);
  const RS_Vector * _p_left  = this->add_point(left);
  const RS_Vector * _p_right = this->add_point(right);

  iterator_base tl_child = append_child(leaf_it, QuadTreeNodeData(_p_tl, _p_top, _p_center, _p_left, QuadTreeLocation(L,T)));  //tl
  iterator_base tr_child = append_child(leaf_it, QuadTreeNodeData(_p_top, _p_tr, _p_right, _p_center, QuadTreeLocation(R,T))); //tr
  iterator_base br_child = append_child(leaf_it, QuadTreeNodeData(_p_center, _p_right, _p_br, _p_bot, QuadTreeLocation(R,B))); //br
  iterator_base bl_child = append_child(leaf_it, QuadTreeNodeData(_p_left, _p_center, _p_bot, _p_bl, QuadTreeLocation(L,B)));  //bl

  tl_child->region()                   = leaf_it->region();
  tl_child->region_intersection_flag() = leaf_it->region_intersection_flag();
  tl_child->line_intersection_flag()   = leaf_it->line_intersection_flag();

  tr_child->region()                   = leaf_it->region();
  tr_child->region_intersection_flag() = leaf_it->region_intersection_flag();
  tr_child->line_intersection_flag()   = leaf_it->line_intersection_flag();

  br_child->region()                   = leaf_it->region();
  br_child->region_intersection_flag() = leaf_it->region_intersection_flag();
  br_child->line_intersection_flag()   = leaf_it->line_intersection_flag();

  bl_child->region()                   = leaf_it->region();
  bl_child->region_intersection_flag() = leaf_it->region_intersection_flag();
  bl_child->line_intersection_flag()   = leaf_it->line_intersection_flag();
}



void QuadTree::balance()
{
  unsigned int flag;

  do
  {
    flag = 0;
    leaf_iterator leaf_it = begin_leaf();
    for(; leaf_it != end_leaf(); ++leaf_it)
    {
      iterator_base leaf = leaf_it;
      int leaf_depth = depth(leaf);

      std::vector<iterator_base> neighbors;

      if(leaf_it->get_location().has_location(T))
        neighbors.push_back(find_neighbor(leaf, B));
      if(leaf_it->get_location().has_location(B))
        neighbors.push_back(find_neighbor(leaf, T));
      if(leaf_it->get_location().has_location(L))
        neighbors.push_back(find_neighbor(leaf, R));
      if(leaf_it->get_location().has_location(R))
        neighbors.push_back(find_neighbor(leaf, L));

      for(unsigned int n=0; n<neighbors.size(); ++n)
      {
        if(!neighbors[n].node) continue;
        int neighbor_depth = depth(neighbors[n]);
        if(leaf_depth-neighbor_depth>1)
        {
          neighbors[n]->divide_flag() = true;
        }
      }
    }

    for(leaf_it = begin_leaf(); leaf_it != end_leaf(); )
    {
      iterator_base this_leaf = leaf_it++;
      if(this_leaf->divide_flag())
      {
        subdivide(this_leaf);
        flag++;
        this_leaf->divide_flag() = false;
      }
    }

  }
  while(flag);

}


bool QuadTree::has_neighbor_intersection_region(iterator_base & leaf_it)
{
  iterator_base left_neighbor = find_neighbor(leaf_it, L);
  if(left_neighbor.node && left_neighbor->region_intersection_flag()==QuadTreeNodeData::INTERSECTION_REGION)
    return true;

  iterator_base right_neighbor = find_neighbor(leaf_it, R);
  if(right_neighbor.node && right_neighbor->region_intersection_flag()==QuadTreeNodeData::INTERSECTION_REGION)
    return true;

  iterator_base top_neighbor = find_neighbor(leaf_it, T);
  if(top_neighbor.node && top_neighbor->region_intersection_flag()==QuadTreeNodeData::INTERSECTION_REGION)
    return true;

  iterator_base bot_neighbor = find_neighbor(leaf_it, B);
  if(bot_neighbor.node && bot_neighbor->region_intersection_flag()==QuadTreeNodeData::INTERSECTION_REGION)
    return true;

  return false;
}


QuadTree::iterator_base QuadTree::find_neighbor(const iterator_base & it, Location x)
{
  std::stack<QuadTreeLocation> path;
  iterator_base q = it;

  while( !is_root(q) )
  {
    QuadTreeLocation loc = q->get_location();
    path.push(loc);
    q=parent(q);
    if( !loc.has_location(x) ) break;
    if( is_root(q) && loc.has_location(x) ) return iterator_base(0);
  };

  while(!path.empty())
  {
    QuadTreeLocation loc = path.top();
    path.pop();
    if(x==L || x==R)
      q = goto_quadtree_child(q, QuadTreeLocation::x_symmetry(loc));
    if(x==T || x==B)
      q = goto_quadtree_child(q, QuadTreeLocation::y_symmetry(loc));
    if(q.node==0 || is_child(q)) return q;
  }

  return q;
}



QuadTree::iterator_base QuadTree::goto_quadtree_child(const iterator_base & it, const QuadTreeLocation & location)
{
  if(it.node==0) return  iterator_base(0);
  for( sibling_iterator child_it= begin(it); child_it!=end(it); ++child_it)
    if( child_it->get_location()==location)
      return iterator_base(child_it);
  return  iterator_base(0);
}


const RS_Vector * QuadTree::add_point(const RS_Vector & point)
{
  //for(unsigned int n=0; n<_points.size(); ++n)
  //  if( point.absolute_fuzzy_equals(*_points[n]) )
  //    return _points[n];
  if(_point_to_id.find(&point) != _point_to_id.end())
    return _point_to_id.find(&point)->first;

  RS_Vector * new_point = new RS_Vector(point);
  _point_to_id[new_point] = _points.size();
  _points.push_back(new_point);
  return _points[_points.size()-1];
}



bool QuadTree::is_line_intersection(const iterator_base & it, const RS_Vector &p1, const RS_Vector &p2 )
{
  const RS_Vector * _p_tr = it->tr();
  const RS_Vector * _p_tl = it->tl();
  const RS_Vector * _p_br = it->br();
  const RS_Vector * _p_bl = it->bl();

  Poly leaf(Point(_p_bl->x, _p_bl->y));
  leaf.add(Point(_p_br->x, _p_br->y));
  leaf.add(Point(_p_tr->x, _p_tr->y));
  leaf.add(Point(_p_tl->x, _p_tl->y));

  if(leaf.has_point(Point(p1.x, p1.y))) return true;
  if(leaf.has_point(Point(p2.x, p2.y))) return true;

  Poly line(Point(p1.x, p1.y));
  line.add(Point(p2.x, p2.y));

  return intersect(leaf, line)>0;
}



QuadTreeNodeData::REGION_INTERSECTION_FLAG QuadTree::region_intersection(const iterator_base & it,
    const std::vector<RS_Vector > & region_contour )
{

  if(region_contour.size()<2) return QuadTreeNodeData::OUT_REGION;

  const RS_Vector * _p_tr = it->tr();
  const RS_Vector * _p_tl = it->tl();
  const RS_Vector * _p_br = it->br();
  const RS_Vector * _p_bl = it->bl();

  //for rubset reasion
  double eps = (*_p_tr - * _p_tl).x/100;

  Poly leaf(Point(_p_bl->x, _p_bl->y));
  leaf.add (Point(_p_br->x, _p_br->y));
  leaf.add (Point(_p_tr->x, _p_tr->y));
  leaf.add (Point(_p_tl->x, _p_tl->y));

  Poly leaf_large(Point(_p_bl->x-eps, _p_bl->y-eps));
  leaf_large.add (Point(_p_br->x+eps, _p_br->y-eps));
  leaf_large.add (Point(_p_tr->x+eps, _p_tr->y+eps));
  leaf_large.add (Point(_p_tl->x-eps, _p_tl->y+eps));

  Poly contour(Point(region_contour[0].x, region_contour[0].y));
  for(unsigned int n=1; n<region_contour.size(); ++n)
    contour.add(Point(region_contour[n].x, region_contour[n].y));

  if(intersect(leaf_large, contour))
  {
    return QuadTreeNodeData::INTERSECTION_REGION;
  }

  // no intersection point?
  if(contour.has_point(Point(_p_bl->x, _p_bl->y)) && contour.has_point(Point(_p_tr->x, _p_tr->y)))
  {
    //assert(fabs(contour.area()) >= leaf.area());
    return QuadTreeNodeData::IN_REGION;
  }

  if(leaf_large.has_point(Point(region_contour[0].x, region_contour[0].y)))
  {
    //assert(leaf.area() >= fabs(contour.area()));
    return QuadTreeNodeData::COVER_REGION;
  }

  return QuadTreeNodeData::OUT_REGION;
}


QuadTreeNodeData::REGION_INTERSECTION_FLAG QuadTree::region_intersection(const iterator_base & it,  const CG_Region & region )
{
  RS_Hatch * hatch = region.hatch;
  if(!hatch->hasHole())
    return region_intersection(it,  region.contour_points );

  const RS_Vector * _p_tr = it->tr();
  const RS_Vector * _p_tl = it->tl();
  const RS_Vector * _p_br = it->br();
  const RS_Vector * _p_bl = it->bl();

  RS_Line line1(0, RS_LineData(*_p_bl, *_p_br));
  if(hatch->hasIntersectionWithEdge(&line1)) return QuadTreeNodeData::INTERSECTION_REGION;

  RS_Line line2(0, RS_LineData(*_p_br, *_p_tr));
  if(hatch->hasIntersectionWithEdge(&line2)) return QuadTreeNodeData::INTERSECTION_REGION;

  RS_Line line3(0, RS_LineData(*_p_tl, *_p_tr));
  if(hatch->hasIntersectionWithEdge(&line3)) return QuadTreeNodeData::INTERSECTION_REGION;

  RS_Line line4(0, RS_LineData(*_p_bl, *_p_tl));
  if(hatch->hasIntersectionWithEdge(&line4)) return QuadTreeNodeData::INTERSECTION_REGION;

  bool this_point_on_hatch;
  if(RS_Information::isPointInsideContour(*_p_bl,  hatch, &this_point_on_hatch))
    return QuadTreeNodeData::IN_REGION;

  if(it->has_point(hatch->getData().internal_point))
    return  QuadTreeNodeData::COVER_REGION;

  return QuadTreeNodeData::OUT_REGION;

  /*
    unsigned int n_point_in_region = 0;
    bool has_point_on_region=false;

    {
      bool this_point_on_hatch;

      n_point_in_region += RS_Information::isPointInsideContour(*_p_tr,  hatch, &this_point_on_hatch);
      if(this_point_on_hatch) has_point_on_region=true;

      n_point_in_region += RS_Information::isPointInsideContour(*_p_tl,  hatch, &this_point_on_hatch);
      if(this_point_on_hatch) has_point_on_region=true;

      n_point_in_region += RS_Information::isPointInsideContour(*_p_br,  hatch, &this_point_on_hatch);
      if(this_point_on_hatch) has_point_on_region=true;

      n_point_in_region += RS_Information::isPointInsideContour(*_p_bl,  hatch, &this_point_on_hatch);
      if(this_point_on_hatch) has_point_on_region=true;
    }

  std::cout<<n_point_in_region<<std::endl;
    if(has_point_on_region)
    {
      std::cout<<"INTERSECTION_REGION"<<std::endl;
      return QuadTreeNodeData::INTERSECTION_REGION;
    }

    if(n_point_in_region==4)
    {
      return QuadTreeNodeData::IN_REGION;
    }

    if(n_point_in_region==0)
    {
      if(it->has_point(hatch->getData().internal_point))
        return  QuadTreeNodeData::COVER_REGION;
      else return QuadTreeNodeData::OUT_REGION;
    }

    if(n_point_in_region>0 && n_point_in_region<4)
      return QuadTreeNodeData::INTERSECTION_REGION;
  */

}



void QuadTree::print_path(const iterator_base & it) const
{
  std::stack<QuadTreeLocation> path;
  iterator_base q = it;

  if(q.node==0) return;

  while( !is_root(q) )
  {
    QuadTreeLocation loc = q->get_location();
    path.push(loc);
    q=parent(q);
  };

  std::cout<<'/';

  while(!path.empty())
  {
    QuadTreeLocation loc = path.top();
    path.pop();
    loc.print(std::cout);
    std::cout<<'/';
  }

  std::cout<<std::endl;
}



void QuadTree::export_quadtree(char * filename)
{

  std::ofstream fout;
  fout.open(filename, std::ofstream::trunc);

  fout << "# vtk DataFile Version 3.0" <<'\n';
  fout << "Date calculated by CTRI"    <<'\n';
  fout << "ASCII"                      <<'\n';
  fout << "DATASET UNSTRUCTURED_GRID"  <<'\n';
  fout << "POINTS " << _points.size()  << " float" << '\n';

  for (unsigned int i=0; i<_points.size(); ++i)
  {
    fout << _points[i]->x << " \t" << _points[i]->y  << " \t " << 0.0 << '\n';
  }

  fout << std::endl;

  unsigned int n_leafs=0;
  leaf_iterator leaf_it = begin_leaf();
  for(; leaf_it != end_leaf(); ++leaf_it)
    n_leafs++;

  fout<<"CELLS "<<n_leafs<<" "<<5*n_leafs<<'\n';

  for(leaf_it = begin_leaf(); leaf_it != end_leaf(); ++leaf_it)
  {
    fout << 4 << " "
    << _point_to_id[leaf_it->tl()] << " "
    << _point_to_id[leaf_it->tr()] << " "
    << _point_to_id[leaf_it->br()] << " "
    << _point_to_id[leaf_it->bl()] << "\n";
  }

  fout << std::endl;

  fout << "CELL_TYPES " << n_leafs << '\n';

  for(unsigned int i=0; i<n_leafs; ++i)
    fout << 9 << std::endl;

  fout << std::endl;


  fout << "CELL_DATA "<<n_leafs     <<'\n';
  fout << "SCALARS region float 1"  <<'\n';
  fout << "LOOKUP_TABLE default"    <<'\n';
  for(leaf_it = begin_leaf(); leaf_it != end_leaf(); ++leaf_it)
  {
    fout << static_cast<int>(leaf_it->region())<<'\n';
  }
  fout<<std::endl;

  fout << "SCALARS region_flag float 1"  <<'\n';
  fout << "LOOKUP_TABLE default"    <<'\n';
  for(leaf_it = begin_leaf(); leaf_it != end_leaf(); ++leaf_it)
  {
    fout << static_cast<int>(leaf_it->region_intersection_flag())<<'\n';
  }
  fout<<std::endl;

  fout.close();


}
