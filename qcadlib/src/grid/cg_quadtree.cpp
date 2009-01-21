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

#include <stack>

#include "cg_quadtree.h"


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

  this->append_child(leaf_it, QuadTreeNodeData(_p_tl, _p_top, _p_center, _p_left, QuadTreeLocation(L,T)));  //tl
  this->append_child(leaf_it, QuadTreeNodeData(_p_top, _p_tr, _p_right, _p_center, QuadTreeLocation(R,T))); //tr
  this->append_child(leaf_it, QuadTreeNodeData(_p_center, _p_right, _p_br, _p_bot, QuadTreeLocation(R,B))); //br
  this->append_child(leaf_it, QuadTreeNodeData(_p_left, _p_center, _p_bot, _p_bl, QuadTreeLocation(L,B)));  //bl

}


void QuadTree::balance()
{








}


QuadTree::iterator_base QuadTree::find_neighbor(iterator_base & q, Location x)
{
  QuadTreeLocation loc = q->get_location();

  std::stack<QuadTreeLocation> path;
  iterator_base p = parent(q);

  while( is_top_parent(p) && loc.has_location(x) )
  {
    path.push(loc);
    q=p;
    p=parent(q);
  }

  while(!path.empty())
  {
    QuadTreeLocation loc = path.top();
    path.pop();
    if(is_child(p)) return p;

    if(x==L || x==R)
      p = to_quadtree_child(p, QuadTreeLocation::x_symmetry(loc));
    if(x==T || x==B)
      p = to_quadtree_child(p, QuadTreeLocation::y_symmetry(loc));
  }

  return p;
}



QuadTree::iterator_base QuadTree::to_quadtree_child(const iterator_base & it, const QuadTreeLocation & location)
{
  for( sibling_iterator child_it= begin(it); child_it!=end(it); ++child_it)
    if( child_it->get_location()==location)
      return iterator_base(child_it);
}


const RS_Vector * QuadTree::add_point(const RS_Vector & point)
{
  for(unsigned int n=0; n<_points.size(); ++n)
    if( point.absolute_fuzzy_equals(*_points[n]) )
      return _points[n];

  RS_Vector * new_point = new RS_Vector(point);
  _points.push_back(new_point);
  return _points[_points.size()-1];
}



