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

#ifndef __cg_quadtree_h__
#define __cg_quadtree_h__

#include <vector>

#include "tree.h"
#include "rs_vector.h"

enum Location{T, B, R, L, TOP};


class QuadTreeLocation
{
public:
  QuadTreeLocation()
  :_x(TOP), _y(TOP)
  {}

  QuadTreeLocation(Location x, Location y)
  :_x(x), _y(y)
  {}

  static QuadTreeLocation x_symmetry(const QuadTreeLocation & location)
  {
    QuadTreeLocation symmetry = location;
    if(location._x == R)
      symmetry._x = L;
    if(location._x == L)
      symmetry._x = R;
    return symmetry;
  }

  static QuadTreeLocation y_symmetry(const QuadTreeLocation & location)
  {
    QuadTreeLocation symmetry = location;
    if(location._y == T)
      symmetry._y = B;
    if(location._y == B)
      symmetry._y = T;
    return symmetry;
  }

  bool has_location(Location loc)
  { return ( _x==loc || _y==loc ); }

  bool operator == (const QuadTreeLocation & other) const
  { return (_x == other._x && _y == other._y); }

private:
  Location _x;
  Location _y;
};


class QuadTreeNodeData
{
public:
  QuadTreeNodeData(const RS_Vector *tl, const RS_Vector *tr,
                   const RS_Vector *br, const RS_Vector *bl,
                   QuadTreeLocation location)
  :_p_tl(tl), _p_tr(tr), _p_br(br), _p_bl(bl), _location(location)
  {}

  QuadTreeNodeData(const QuadTreeNodeData & other)
  {
    _p_tr = other._p_tr;
    _p_tl = other._p_tl;
    _p_br = other._p_br;
    _p_bl = other._p_bl;
    _location = other._location;
  }

  const RS_Vector * tl() const
    { return _p_tl; }

  const RS_Vector * tr() const
    { return _p_tr; }

  const RS_Vector * br() const
    { return _p_br; }

  const RS_Vector * bl() const
    { return _p_bl; }

  const QuadTreeLocation & get_location() const
  { return _location; }

private:

  /**
   * top left point;
   */
  const RS_Vector * _p_tl;

  /**
   * top right point
   */
  const RS_Vector *  _p_tr;

  /**
   * bot right point
   */
  const RS_Vector * _p_br;

  /**
   * bot left point
   */
  const RS_Vector * _p_bl;

  /**
   * the location of quadtree leaf
   */
  QuadTreeLocation _location;

};



class QuadTree : public tree<QuadTreeNodeData>
{
public:
  QuadTree(const QuadTreeNodeData & data)
      :tree<QuadTreeNodeData>(data)
  {}

  void subdivide(iterator_base & leaf_it);

  void balance();

private:
  iterator_base to_quadtree_child(const iterator_base & it, const QuadTreeLocation & location);

  iterator_base find_neighbor(iterator_base & it, Location x);

  const RS_Vector * add_point(const RS_Vector & point);

  std::vector<const RS_Vector *> _points;
};


#endif
