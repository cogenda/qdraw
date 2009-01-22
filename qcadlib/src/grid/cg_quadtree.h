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
#include <map>

#include "tree.h"
#include "rs_vector.h"

enum Location{T, B, R, L, ROOT};


class QuadTreeLocation
{
public:
  QuadTreeLocation()
  :_x(ROOT), _y(ROOT)
  {}

  QuadTreeLocation(Location x, Location y)
  :_x(x), _y(y)
  {}

  static QuadTreeLocation x_symmetry(const QuadTreeLocation & location)
  {
    QuadTreeLocation symmetry = location;
    if(location._x == R)
      symmetry._x = L;
    else if(location._x == L)
      symmetry._x = R;
    return symmetry;
  }

  static QuadTreeLocation y_symmetry(const QuadTreeLocation & location)
  {
    QuadTreeLocation symmetry = location;
    if(location._y == T)
      symmetry._y = B;
    else if(location._y == B)
      symmetry._y = T;
    return symmetry;
  }

  bool has_location(Location loc) const
  { return ( _x==loc || _y==loc ); }

  bool operator == (const QuadTreeLocation & other) const
  { return (_x == other._x && _y == other._y); }

  void print(std::ostream& os) const
  {
    if(_y == T) os << "T";
    if(_y == B) os << "B";
    if(_x == R) os << "R";
    if(_x == L) os << "L";
  }
  	
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
  :_p_tl(tl), _p_tr(tr), _p_br(br), _p_bl(bl), 
   _location(location), _divide_flag(false), 
   _area_constrain_ok(false), _region(-1)
  {}

  QuadTreeNodeData(const QuadTreeNodeData & other)
  {
    _p_tr = other._p_tr;
    _p_tl = other._p_tl;
    _p_br = other._p_br;
    _p_bl = other._p_bl;
    _location = other._location;
    _divide_flag = other._divide_flag;
    _area_constrain_ok = other._area_constrain_ok;
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

  double area() const
  {
    RS_Vector X = (*_p_tr - *_p_tl); 
    RS_Vector Y = (*_p_tr - *_p_br); 
    return X.x * Y.y;
  }
  
  bool & divide_flag() 
  { return _divide_flag; }
    
  const bool & divide_flag() const
  { return _divide_flag; }

  void set_in()
  { _flag = IN; }
  
  bool is_in() const
  { return _flag == IN; }
  
  void set_out()
  { _flag = OUT; }

  bool is_out() const
  { return _flag == OUT; }
  
  void set_mix()
  { _flag = MIX; }
  
  bool is_mix() const
  { return _flag == MIX; }
  
  void toggle_area_constrain_satisfied()
  { _area_constrain_ok = true; }
  
  bool is_area_constrain_satisfied() const
  { return _area_constrain_ok; }
  
  /**
   * read/write region info
   */
  int & region()
  { return _region; }
  
  friend std::ostream& operator << (std::ostream&, const QuadTreeNodeData& data);

private:
  
  enum CLIPFLAG{IN, OUT, MIX};	
  
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

  /**
   * indicate if this leaf should be divide
   */
  bool _divide_flag;

  /**
   * indicate that area constrain has been satisfied
   */
  bool _area_constrain_ok;
  
  /**
   * when CLIPFLAG _flag is IN, 
   * indicate this quadtree leaf in which region
   */
  int _region;
  
  /**
   * region (as polygon) and quadtree leaf clip result 
   */
  CLIPFLAG _flag;

};



class QuadTree : public tree<QuadTreeNodeData>
{
public:

  /**
   * construct empty quadtree
   */
  QuadTree() {}	


  QuadTree(const QuadTreeNodeData & data)
      :tree<QuadTreeNodeData>(data)
  {}
  
  /**
   * free _points array
   */
  ~QuadTree();
  
  /**
   * divide leaf into 4 child leaf
   */
  void subdivide(iterator_base & leaf_it);

  /**
   * balance the quadtree (neighbor leaf only have one depth difference )
   */
  void balance();

  /**
   * add point to quadtree. 
   * we will first search _points array to see if \p point already exist 
   * or we will create new RS_Vector by the \p point and strore the new created point in _points
   * for both situation, return the pointer point to  _points
   */
  const RS_Vector * add_point(const RS_Vector & point);

  /**
   * find the neighbor of leaf in the direction  x
   * return empty iterator if no find
   */
  iterator_base find_neighbor(const iterator_base & it, Location x);

  /**
   * write quadtree mesh in vtk format 
   */
  void export_quadtree(char * file);
  
  /**
   * print the absolute path of an iterator 
   */ 
  void print_path(const iterator_base & it) const;

private:  
	
  iterator_base goto_quadtree_child(const iterator_base & it, const QuadTreeLocation & location);
  
  std::vector<const RS_Vector *> _points;

  std::map<const RS_Vector *, unsigned int> _point_to_id;
};


#endif