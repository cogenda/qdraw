static const char rcs_id[] = "$Header: /cvsroot/clippoly/clippoly/nclip.cc,v 1.5 2005/02/28 17:21:12 klamer Exp $";

//    nclip: a polygon clip library

//    Copyright (C) 1993  University of Twente

//    klamer@mi.el.utwente.nl

//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.

//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.

//    You should have received a copy of the GNU Library General Public
//    License along with this library; if not, write to the Free
//    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

// $Log: nclip.cc,v $
// Revision 1.5  2005/02/28 17:21:12  klamer
// Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
// Change use of (libg++) String to ANSI C++ string.
//
// Revision 1.2  1994/01/04  12:55:37  klamer
// Added DEBUG_NCLIP support.
// Make intersect call itself again if the polygons change.
// New function label_shared2, called from label_shared.
// Major changes in label_shared.
// Make_poly depends on inproducts now rather than on angles.
// Make sure starting polygons are clockwise oriented.
//
// Revision 1.1  1992/12/07  10:46:35  klamer
// Initial revision
//

#ifdef __GNUG__
#pragma implementation
#endif

#include	<assert.h>

#include	<graphmat.h>
#include	<graphadd.h>
#include        <err.h>

#include	<malloc.h>

#include	<poly.h>
#include	<primitives.h>
#include	<posadder.h>
#include	"nclip.h"

#ifdef DEBUG1
#include	<iostream.h>
#include	<poly_io.h>
#endif
#ifdef DEBUG_NCLIP
#include        "poly_io.h"
#include        <iostream.h>
#include        <fstream.h>
#include        <iomanip.h>
static ofstream	db("debugnc");
#endif

static const char h_rcs_id[] = NCLIP_H;

// Intersect Edge a & b, and return the intersection point in p1 and p2.
// The return value means the number of intersections. Two intersections
// mean that a and b are on the same line, the intersection points are in that
// case the extrema of the joint line segment
int
intersect( const Edge &a, const Edge &b, Point &p1, Point &p2 )
{
  hvec2_t	s1, s2;

  int ret = v_inters2( a.p1().hvec(), a.p2().hvec(), b.p1().hvec(), b.p2().hvec(), &s1, &s2 );

  switch(ret)
  {
  case 2:
    p2 = s2;
    // Fall Through
  case 1:
    p1 = s1;
  }

  return ret;
}

// calculate all the intersections between a and b. Add the intersection points
// to a and b.
// returns total number of intersections found.
int
intersect( Poly &a, Poly &b )
{
  PolyIter	a_iter( a );
  int			res = 0, res2 = 0;

  while( a_iter() )
  {
    PolyIter	b_iter( b );

    while( b_iter() )
    {
      Point	intersect1, intersect2;
      int		cnt = intersect( a_iter.edge(), b_iter.edge(),
                            intersect1, intersect2 );
      res += cnt;

      if (cnt)
      {
        res2 += PolyIter::add_point( a_iter, b_iter, intersect1 );
        if (cnt >= 2)
        {
          res2 += PolyIter::add_point( a_iter, b_iter, intersect2 );
        }
      }
    }
  }

  if (res2 != 0)
    return res + intersect(a,b);

  return res;
}

// Check whether line p1-p2 is inside the polygon pointed by pn. p1 should equal pn.
// if inside do a_iter.set_inside() else a_iter.set_none(), as it is outside
void
label_shared2( PolyIter &pi, const Point &p1, const Point &p2, const PolyNode &pn )
{
  assert(p1 == pn.point());

  Point		prev = pn.prevnode().point(),
                next = pn.nextnode().point();

  // The *dir are direction vectors!
  Point		cdir = p2 - p1, pdir = p1 - prev, ndir = next - p1;

  int		p_c = (pdir.x() * cdir.y() - pdir.y() * cdir.x()) >= 0,
             n_c = (ndir.x() * cdir.y() - ndir.y() * cdir.x()) >= 0,
                   p_n = (pdir.x() * ndir.y() - pdir.y() * ndir.x()) >= 0;

  if (p_n)	// pn points to a concave corner?
  {
    if (p_c && n_c)
      pi.set_none();
    else
      pi.set_inside();
  } else
  {
    if (p_c || n_c)
      pi.set_none();
    else
      pi.set_inside();
  }
}

// label the edges from polygon a with respect to polygon b. possible
// labels are: shared, inside and none. inside means that the edge is inside
// b, none means outside, and shared means that b has the same edge.
void
label_shared( Poly & a, const Poly & b )
{
  PolyIter	a_iter( a );

  while(a_iter())
  {
    const PolyNode *b1, *b2;

    if ((b1 = a_iter.node()->link()) &&
        (b2 = a_iter.nextnode()->link()))
    {
      // Seems shared -- but check
      // that b1 and b2 are also connected!
      if ((b.nextnode(b1) == b2) || (b.nextnode(b2) == b1))
      {
        a_iter.set_shared();
        continue;
      } /* else if (b.has_point( a_iter.edge().middle() ))
            				a_iter.set_inside();
            			else
            				a_iter.set_none(); */
      //continue;
    }
#ifdef notdef
    if (b1)
    {
      // So a_iter.node() shares a point with b
      if (b.has_point(a_iter.nextnode().point()))
        a_iter.set_inside();
      else
        a_iter.set_none();
    }
    else
    {
      // The current node is not on b
      if (b.has_point(a_iter.node().point()))
        a_iter.set_inside();
      else
        a_iter.set_none();
    }

    //if (b.has_point( a_iter.edge().middle() ))
    //	a_iter.set_inside();
    //else
    //	a_iter.set_none();
#else
    if (b1)
      label_shared2(a_iter, a_iter.node()->point(), a_iter.nextnode()->point(), *b1 );
    //warning("NYI %s %d\n", __FILE__, __LINE__);
    else if ((b2 = a_iter.nextnode()->link()))
      label_shared2(a_iter, a_iter.nextnode()->point(), a_iter.node()->point(), *b2 );
    //warning("NYI %s %d\n", __FILE__, __LINE__);
    else
    {
      int in1 = b.has_point(a_iter.node()->point()),
                in2 = b.has_point(a_iter.nextnode()->point());

      if (in1 != in2)
        error("This should not happen! %s %d\n", __FILE__, __LINE__ );
      if (in1)
        a_iter.set_inside();
      else
        a_iter.set_none();
    }
#endif

  }
}

// In the two make_poly functions (second one below this one) a new
// polygon is added to polylist. done contains a list of edge which
// already are assigned to a polygon, and thus do not belong to this
// polygon (as the directed edge a->b is only part of one clockwise
// oriented polygon.) The second make_poly function is a recursive function
// which finishes the work initiated in the first.
void
make_poly( const Point &point,
           DirPolyIter &follow, PolyPList &polylist, NodePEdgeList &done )
{
  NodePEdge	edge( follow );

  if (done.contains(edge))	// Ignore if this edge already is done.
    return;

  // Make new polygon. start with only one point & edge
  Poly	*new_poly = new Poly( follow.point(), &follow.poly(),
                             follow.edgestate() );

  // finish the construction of the polygon.
  if (make_poly( point, point, follow, done, new_poly ))
  {
    // We want only clockwise oriented polygons!
    if (new_poly->orientation() != ClockWise)
      delete new_poly;
    else
      polylist.add( new_poly );
  }
  else // Will never happen when only one is returned...
    delete new_poly;
}

// second part of make_poly. Continue adding points to new_poly until
// start_point is reached, and thus the polygon is complete. follow is used
// to get the next point of the polygon. If the next point is an intersection
// between the two original polygons, decide whether the other polygon
// should be followed. This is done by selecting the smallest angle.
int
make_poly( const Point &start_point, const Point &point,
           DirPolyIter &follow, NodePEdgeList &done, Poly * new_poly )
{
  done.add( NodePEdge( follow ) );

  follow.next();

  // if (follow.point() == start_point)
  //	return 1;

  double	cur_angle = angle( point, follow.point(), follow.nextpoint() );

  assert(cur_angle != 0);

  IterDirection	dir = NONE;

  if (follow.link())
  {
    double	other_angle = angle( point, follow.point(),
                                follow.linknextpoint() );
    Point	d1 = follow.linknextpoint() - follow.point(),
               d2 = follow.nextpoint() - follow.point(),
                    d3 = follow.point() - point;
    double inp = d1.x() * d2.y() - d2.x() * d1.y(),
                 inp2 = d1.x() * d3.y() - d3.x() * d1.y();
    int	lf = inp > 0,
             ld = inp2 > 0,
                  df = (d3.x() * d2.y() - d2.x() * d3.y()) > 0,
                       condition = (lf + ld + df) >= 2;
    // if (inp2 = 0)	// Similar to other_angle = 0
    if (point == follow.linknextpoint())
      condition = 0;
    assert((cur_angle != other_angle) ||
           (inp != 0) ||
           (follow.linknextpoint() == follow.nextpoint()));
    assert((other_angle != 0) ||
           (inp2 != 0) ||
           (follow.linknextpoint() == point));
    //assert((cur_angle == other_angle) ||
    //       ((((cur_angle - other_angle) > 0))
    //	^ condition));
#ifdef notdef
    if ((other_angle != 0) && (other_angle == cur_angle ?
                               inp > 0 :
                               other_angle < cur_angle))
    {
      assert(condition);
#else
    if (condition)
    {
#endif
      cur_angle = other_angle;
      dir = FORWARD;
      d2 = d1;
    } //else
    //assert(!condition);

    other_angle = angle( point, follow.point(),
                         follow.linkprevpoint() );
    d1 = follow.linkprevpoint() - follow.point();
    //	d2 = follow.nextpoint() - follow.point();
    inp = d1.x() * d2.y() - d2.x() * d1.y();
    inp2 = d1.x() * d3.y() - d3.x() * d1.y();
    lf = inp > 0;
    ld = inp2 > 0;
    df = (d3.x() * d2.y() - d2.x() * d3.y()) > 0;
    condition = (lf + ld + df) >= 2;
    // if (inp2 = 0)
    if (point == follow.linkprevpoint())
      condition = 0;
    assert((cur_angle != other_angle) ||
           (inp != 0) ||
           (follow.linkprevpoint() == follow.nextpoint()));
    assert((other_angle != 0) ||
           (inp2 != 0) ||
           (follow.linkprevpoint() == point));
    //assert((cur_angle == other_angle) ||
    //       ((((cur_angle - other_angle) > 0))
    //	^ condition));
#ifdef notdef
    if ((other_angle != 0) && (other_angle == cur_angle ?
                               inp > 0 :
                               other_angle < cur_angle))
    {
      assert(condition);
#else
    if (condition)
    {
#endif
      // No need to update cur_angle
      dir = BACKWARD;
    } //else
    // assert(!condition);
  }

  if (dir != NONE)
  {

    DirPolyIter	next_iter( follow, dir );

    if (follow.point() == start_point)
      if (new_poly->nextnode(new_poly->firstnode())->point() == next_iter.nextpoint())
        return 1;

    new_poly->add( next_iter.point(), &next_iter.poly(),
                   next_iter.edgestate() );

    return make_poly( start_point, follow.point(), next_iter,
                      done, new_poly );
  }
  else
  {	// Continue using follow
    if (follow.point() == start_point)
      if (new_poly->nextnode(new_poly->firstnode())->point() == follow.nextpoint())
        return 1;

    new_poly->add( follow.point(), &follow.poly(),
                   follow.edgestate() );

    return make_poly( start_point, follow.point(), follow,
                      done, new_poly );
  }
}

// a and b are two polygons with their intersection points added.
// return in polylist all polygons which are 1) inside a and b or 2)
// inside a and outside b or 3) outside a and inside b
void
iter_mesh( const Poly &a, const Poly &b, NodePEdgeList &done,
           PolyPList &polylist )
{
  ConstPolyIter	iter( a );
  int				first = 1;

  while(iter())
  {
    const PolyNode	*cur = iter.node();

    DirPolyIter	a_iter( a, cur, b, FORWARD );
    make_poly( cur->point(), a_iter, polylist, done );
    if (first)
    {
      // On first point in a polygon you should go backwards
      first = 0;
      DirPolyIter	a_bw_iter( a, cur, b, BACKWARD );
      make_poly( cur->point(), a_bw_iter, polylist, done );
    }

    if (cur->link())
    {
      DirPolyIter	b_fw_iter( b, cur->link(), a, FORWARD );
      if (b_fw_iter.edgestate() != Shared)
        make_poly( cur->point(), b_fw_iter, polylist,
                   done );

      DirPolyIter	b_bw_iter( b, cur->link(), a, BACKWARD);
      if (b_bw_iter.edgestate() != Shared)
        make_poly( cur->point(), b_bw_iter, polylist,
                   done );
    }
  }
}

// Determine in which class the polygons in in_list fall. This can be
// inside a but outside b; than the poygon will be put in a_min_b.
// Inside b but outside a will put the the polygon in b_min_a.
// Inside both a and b will put the polygon in a_and_b.
void
assign_polys( const Poly &a, const Poly &b, const PolyPList &in_list,
              PolyPList &a_min_b, PolyPList &b_min_a, PolyPList &a_and_b )
{
#ifdef DEBUG1
  cout << "a: " << &a << endl;
  cout << a;
  cout << "b: " << &b << endl;
  cout << b;
#endif
  PolyPListIter	iter( in_list );

  while(iter())
  {
    Poly	*poly = iter.val();
    ConstPolyIter	p_i( *poly );
    // PosAdder has a class with the states True, False and Unkown.
    // An error (exception) will occur if True of False is set()
    // if its state is not Unknown.
    PosAdder	a_parent, b_parent;

#ifdef DEBUG1
    cout << *poly;
#endif
    while(p_i())
    {
#ifdef DEBUG1
      cout << (*p_i.node()) << p_i.parent(a) << ',' << p_i.parent(b) << endl;
#endif
      // The next two statements collect evidence about
      // the parenthood. This is done on the basis of
      // the edge state (shared, none, inside).
      a_parent.set(p_i.parent(a));
      b_parent.set(p_i.parent(b));
    }
#ifdef notdef
    if (a_parent() == True && b_parent() == True)
      a_and_b.add(poly);
    else if (a_parent() == True)
      a_min_b.add(poly);
    else if (a_parent() == UnKnown && b_parent() == UnKnown)
      // a and b had all sides shared --
      // so probably they are similar
      a_and_b.add(poly);
    else
    {
      assert( b_parent() == True );
      b_min_a.add(poly);
    }
#else
    if (a_parent() == TrueFalse || b_parent() == TrueFalse)
      // probably not a nor b -- so skip
      delete poly;
    else if (a_parent() != False && b_parent() != False)
      a_and_b.add(poly);
    else if (a_parent() == True)
      a_min_b.add(poly);
    // else if (a_parent() == UnKnown && b_parent() == UnKnown)
    //	// a and b had all sides shared --
    //	// so probably they are similar
    //	a_and_b.add(poly);
    else
    {
      // If the next assertion failes it could be that
      // the polygon is not a and not b.
      assert( b_parent() == True );
      b_min_a.add(poly);
    }
#endif

  }
}

void
poly_min_poly( const Poly &a, const Poly &b, PolyPList &a_min_b )
{
  // b is inside a.
  // This means we have to split a, as we don't allow polygons with holes.
  // Split a in three parts: left of b, b and right of b
  // Split over line top from b, bottom from b.

  Point			top(b.firstpoint()), bottom(b.firstpoint());
  const PolyNode	*top_node = b.firstnode(),
                             *bottom_node = b.firstnode();

  ConstPolyIter	b_iter(b);

  while(b_iter())
    if (b_iter.point() > top)
    {
      top = b_iter.point();
      top_node = b_iter.node();
    }
    else if (b_iter.point() < bottom)
    {
      bottom = b_iter.point();
      bottom_node = b_iter.node();
    }

  assert( top_node != bottom_node );

  // Find an element of a, so that a line from that point to top does
  // not intersect with another edge of a, and this point is bigger as
  // top
  // Do the same for bottom, at the same time.

  ConstPolyIter	a_iter(a);
  const PolyNode	*a_top_node = 0, *a_bottom_node = 0;

  while(a_iter())
  {
    Point	cur(a_iter.point());

    if (!(	((cur > top) && (a_top_node == 0)) ||
           ((cur < bottom) && (a_bottom_node == 0)) ))
      continue;

    Edge	edge(cur, ((cur > top) ? top : bottom));

    ConstPolyIter	a_iter2(a);

    int do_intersect = 0;

    while(a_iter2())
    {
      if ((a_iter2.point() == cur) || (a_iter2.nextpoint() == cur))
        continue;
      Point	dummy1,dummy2;
      if (intersect(edge,Edge(a_iter2.point(),a_iter2.nextpoint()),
                    dummy1, dummy2 ))
      {
        do_intersect = 1;
        break;
      }
    }
    if (do_intersect == 0)
      if (cur > top)
        a_top_node = a_iter.node();
      else
        a_bottom_node = a_iter.node();

    if ((a_top_node != 0) && (a_bottom_node != 0))
      break;
  }

  assert((a_top_node != 0) && (a_bottom_node != 0));
  assert(a_top_node->point() > top);
  assert(a_bottom_node->point() < bottom);

  Poly	*left = new Poly(a_top_node->point());

  DirPolyIter	b_back(b,top_node,a,BACKWARD);

  add_until( left, bottom, b_back );
  left->add(bottom);

  DirPolyIter a_forw(a,a_bottom_node,b,FORWARD);

  add_until( left, a_top_node->point(), a_forw);

  a_min_b.add( left );

  Poly	*right = new Poly(top);

  add_until( right, a_bottom_node->point(), a_forw );
  right->add(a_bottom_node->point());
  add_until( right, top, b_back );

  a_min_b.add(right);
}

void
add_until( Poly *polyp, const Point &point, DirPolyIter &dpi )
{
  do
  {
    polyp->add(dpi.point());
    dpi.next();
  }
  while(dpi.point() != point);
}

// clip the polygons a_org and b_org in the classes a_min_b, b_min_a and
// a_and_b (see above for the meanings).
void
clip_poly( const Poly &a_org, const Poly &b_org,
           PolyPList &a_min_b, PolyPList &b_min_a, PolyPList &a_and_b )
{
  Poly	a( a_org ), b( b_org );	// a and b can change, so protect

  a.make_prev();
  if (a.orientation() != ClockWise)
  {
    a.revert();
    a.make_prev();
  }
  b.make_prev();
  if (b.orientation() != ClockWise)
  {
    b.revert();
    b.make_prev();
  }

  int	nr_intersect = intersect( a, b );

  if (nr_intersect == 0)
  {
    // Two case are possible: a and b are disjunct,
    // or one is inside the other.
    if (a.has_point(b.firstpoint()))
    {
      // b is inside a
      a_and_b.add( new Poly(b) );
      poly_min_poly( a, b, a_min_b );
    }
    else if (b.has_point(a.firstpoint()))
    {
      // a inside b
      a_and_b.add( new Poly(a) );
      poly_min_poly( b, a, b_min_a );
    }
    else
    {
      // a and b are disjunct
      a_min_b.add( new Poly(a) );
      b_min_a.add( new Poly(b) );
    }

    return;
  }

  label_shared( a, b );
  label_shared( b, a );
#ifdef DEBUG_NCLIP
  // static ofstream	db("debugnc");
  ostream	&alias = db;
  alias << setprecision(20);
  db << "a:\n" << a;
  db << "b:\n" << b;
#endif

  NodePEdgeList	done(100);
  PolyPList		polylist;

  iter_mesh( a, b, done, polylist );
  iter_mesh( b, a, done, polylist );

  assign_polys( a, b, polylist, a_min_b, b_min_a, a_and_b );
}

