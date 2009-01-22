static const char rcs_id[] = "$Header: /cvsroot/clippoly/clippoly/poly.cc,v 1.7 2005/03/11 14:18:21 klamer Exp $";

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

// $Log: poly.cc,v $
// Revision 1.7  2005/03/11 14:18:21  klamer
// Fixed namespace clash for abs(double) happening in RedHat 7.2 linux i386.
//
// Revision 1.6  2005/02/28 21:12:05  klamer
// Made changes such that gcc 3.4.2 compiles silent with -ansi -pedantic -Wall.
//
// Revision 1.5  2005/02/28 17:21:12  klamer
// Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
// Change use of (libg++) String to ANSI C++ string.
//
// Revision 1.2  1994/01/04  12:55:37  klamer
// Made copy constructor of PolyNode a dummy, and added one which also
// sets parent.
// Make orientation use area rather than angles.
// Added Poly::revert() member.
// PolyIter::add: start with check whether current point already is in
// polygon.
// PolyIter::add_point(): now returns number of nodes added.
// ConstPolyIter::parent(const Poly &): Changed conditions.
//
// Revision 1.1  1993/10/27  14:44:07  klamer
// Initial revision
//
// Revision 1.1  1992/12/07  10:46:35  klamer
// Initial revision
//

#ifdef __GNUG__
#pragma implementation
#endif

#include	<assert.h>

#include	<err.h>

#include	"poly.h"
#include	"posadder.h"
//#include	"boundingbox.h"
//#include	"poly_use.h"

static const char h_rcs_id[] = POLY_H;

#ifndef M_PI
#define M_PI            3.14159265358979323846  /* pi */
#endif

PolyNode::PolyNode( const PolyNode &)
{
  // Argh! use constructor which sets parent!
  error("This should not happen %s %d\n", __FILE__, __LINE__ );
}

PolyNode::PolyNode( const PolyNode &copy , const Poly *parent )
    : p(copy.p), prev(0), _link(0), _parent_poly( parent), _edgestate( Unknown )
{
	if (copy.next)
	  next = new PolyNode( *copy.next, parent );
	else
		next = 0;
}

PolyNode::~PolyNode()
{
	if (next)
		delete next;
}

const Poly *
PolyNode::parent_poly() const
{
	assert(_parent_poly != 0);
	
	return _parent_poly;
}

NodePEdge::NodePEdge( const DirPolyIter &dpi )
{
	const PolyNode	*node = dpi.node(),
					*link = node->link();
	
	if (link)
		if (link < node)
			n1 = link;
		else
			n1 = node;
	else
		n1 = node;

	node = dpi.nextnode();
	link = node->link();
	
	if (link)
		if (link < node)
			n2 = link;
		else
			n2 = node;
	else
		n2 = node;
}

void
Poly::make_prev() const
{
	PolyIter	iter(*(Poly *)this);
	PolyNode	*last = 0;
	
	while(iter())
	{
		iter.node()->prev = last;
		last = iter.node();
	}
	
	((Poly *)this)->prev = last;
}

const PolyNode *
Poly::nextnode(const PolyNode *node) const
{
	if (node->next)
		return node->next;

	return list;
}

const PolyNode *
Poly::prevnode(const PolyNode *node) const
{
	assert(prev);
	
	if (node->prev)
		return node->prev;

	return prev;
}

void
Poly::add( const Point &p, const Poly *parent, EdgeState edgestate )
{
	if (!prev)
		make_prev();
		
	PolyNode	*new_node = new PolyNode( p, parent, edgestate );
	
	new_node->prev = prev;
	new_node->next = 0;
	
	assert( prev->next == 0 );
	prev->next = new_node;
	prev = new_node;
}
	
Orientation
Poly::orientation() const
{
	if (!prev)
		make_prev();
		
	ConstPolyIter	iter(*this);
	
	double	tot_angle = 0;
	double	area = 0;
	Point	first;
	int	first_flag = 1;
	
	while(iter())
	{
		tot_angle += angle( iter.prevpoint(), iter.point(), 
						iter.nextpoint() ) - M_PI;
		if (first_flag)
		{
			first_flag = 0;
			first = iter.point();
			continue;
		}
		// Point	&p1 = iter.point(), &p2 = iter.nextpoint();
		// NOTE: last step in iter also is not needed!
		Point	p1 = iter.point() - first, 
			p2 = iter.nextpoint() - first;
		area += p1.x() * p2.y() - p2.x() * p1.y();
	}
	area /= 2.0;
	
	assert(area != 0);
	assert((area * tot_angle > 0) ||
	       (fabs(fabs(tot_angle) - M_PI * 2.0) > 0.0001));

#ifndef notdef
	if (area < 0)
		return ClockWise;
	else
		return CounterClockWise;
#else
	if (tot_angle < 0)
	{
		assert(tot_angle < -M_PI * 1.9999);
		assert(tot_angle > -M_PI * 2.0001);

		return ClockWise;
	} else {
		assert(tot_angle > M_PI * 1.9999);
		assert(tot_angle < M_PI * 2.0001);

		return CounterClockWise;
	}
#endif
}


double
Poly::area() const
{
	if (!prev)
		make_prev();
		
	ConstPolyIter	iter(*this);
	
	double	ret = 0;
	
	Point	first;
	int	first_flag = 1;
	
	while(iter())
	{
		if (first_flag)
		{
			first_flag = 0;
			first = iter.point();
			continue;
		}
		Point	p1 = iter.point() - first, 
			p2 = iter.nextpoint() - first;
		ret += p1.x() * p2.y() - p2.x() * p1.y();
	}

	return ret / 2.0;
}

void
Poly::revert()
{
  if (!prev)
    make_prev();
  
  PolyNode	*next;
  
  for(PolyNode *cur = list; cur != 0; cur = next)
  {
    next = cur->next;
    cur->next = cur->prev;
    cur->prev = next;
  }
  
  PolyNode	*tmp = prev;
  prev = list;
  list = tmp;
  
  // warning("NYI %s %d\n", __FILE__, __LINE__);
}

int
intersect_right( const Edge &edge, const Point &point )
{
	if ((edge.p1().y() >= point.y()) && (edge.p2().y() >= point.y()))
		return 0;
	if ((edge.p1().y() < point.y()) && (edge.p2().y() < point.y()))
		return 0;
		
	if (edge.p1().y() == edge.p2().y())
		return 0;		// Tricky! This could give errors!
		
	double x = (point.y() - edge.p1().y()) * 
		(edge.p2().x() - edge.p1().x()) / (edge.p2().y() - edge.p1().y())
		+ edge.p1().x();
		
	if (x > point.x())	// Tricky! Is >= better?
		return 1;
	return 0;
}

// is point inside *this?
int	
Poly::has_point( const Point &point ) const
{
	PolyIter	iter( *(Poly *)((void *)this) );
	int		cnt = 0;
	
	while(iter())
		cnt += intersect_right( iter.edge(), point );
		
	return cnt % 2; 
}

double
Poly::xmin() const
{
	ConstPolyIter	iter(*this);
	
	iter();
	double	res = iter.point().x();

	while(iter())
	{
		double	c = iter.point().x();
		if (c < res)
			res = c;
	}
	
	return res;
}

double
Poly::xmax() const
{
	ConstPolyIter	iter(*this);
	
	iter();
	double	res = iter.point().x();

	while(iter())
	{
		double	c = iter.point().x();
		if (c > res)
			res = c;
	}
	
	return res;
}

double
Poly::ymin() const
{
	ConstPolyIter	iter(*this);
	
	iter();
	double	res = iter.point().y();

	while(iter())
	{
		double	c = iter.point().y();
		if (c < res)
			res = c;
	}
	
	return res;
}

double
Poly::ymax() const
{
	ConstPolyIter	iter(*this);
	
	iter();
	double	res = iter.point().y();

	while(iter())
	{
		double	c = iter.point().y();
		if (c > res)
			res = c;
	}
	
	return res;
}

PolyIter::PolyIter( Poly &_poly )
	: poly(_poly), app_next(poly.list)
{ 
}

int
PolyIter::operator() ()
{
	cur = app_next;
	
	if (cur != 0)
	{
		app_next = cur->next;
		return 1;
	} else
		return 0;
}

PolyNode *	
PolyIter::add( const Point &point, int &new_point )
{
  PolyIter	chk(poly);
  
  while(chk())
    if (point == chk.node()->point())
      return chk.node();
  
	if (point == cur->p)
	  warning("This should not happen! %s, %d\n", __FILE__, __LINE__ );
		// return cur;
		
	double	d = len( cur->p - point );
	assert( d <= len( cur->p - AppNext()->p ) );
	PolyNode	*last = cur, *p = next( cur );
	
	for(  ;d > len( cur->p - p->p ); last = p, p = next( p ))
	  ;	// Go to next node, as next node is on same side as current node

	if (p->p == point)
	  warning("This should not happen! %s, %d\n", __FILE__, __LINE__ );
		// return p;
			
	p = new PolyNode( point, &poly, last->next );
	new_point++;
	last->next = p;
	if (p->next)
		p->next->prev = p;
	else if (poly.prev)
	{
		assert(poly.prev == last);
		poly.prev = p;
	}
	p->prev = last;
	
	return p;
}

int	
PolyIter::add_point( PolyIter &a, PolyIter &b, const Point &p )
{
  int	res = 0;
  
	PolyNode	*node_a = a.add( p, res );
	PolyNode	*node_b = b.add( p, res );

	assert((node_a->_link == 0) || (node_a->_link == node_b));	
	node_a->_link = node_b;
	assert((node_b->_link == 0) || (node_b->_link == node_a));	
	node_b->_link = node_a;
	
	return res;
}

ConstPolyIter::ConstPolyIter( const Poly &_poly )
	: polyiter( *(Poly *)(void *)&_poly )
{ 
}

LogicStates
ConstPolyIter::parent(const Poly &poly)
{
	switch(node()->edgestate())
	{case None:
		if (&poly == node()->parent_poly())
			return True;
		else
		  {
		    if (node()->link() != 0)
		      return UnKnown; // Prob. True
		    else
		      return False;
		  }
	case Shared:
		return UnKnown;
	case Inside:
		if (&poly == node()->parent_poly())
			return UnKnown;
		else {
		  if (node()->link() != 0)
		    return UnKnown;
		  else
		    return True;
		}
	case Unknown:
	default:
		fatal("This should not happen %s %d\n", __FILE__, __LINE__);
	}
	
	return UnKnown;
}

DirPolyIter::DirPolyIter( const Poly &poly, const PolyNode *node,
								const Poly &link, IterDirection dir )
	: _poly(poly), _linkpoly(link), _dir(dir), _node(node)
{ 
	if (! _poly.prev)
		_poly.make_prev();
	if (!_linkpoly.prev)
		_linkpoly.make_prev();
}

DirPolyIter::DirPolyIter( const DirPolyIter &dpi, IterDirection dir )
	: _poly(dpi.linkpoly()), _linkpoly( dpi._poly ), _dir(dir), 
	  _node( dpi.node()->link() )
{ 
	if (! _poly.prev)
		_poly.make_prev();
	if (!_linkpoly.prev)
		_linkpoly.make_prev();
}

const PolyNode *
DirPolyIter::nextnode() const
{
	switch(dir())
	{case FORWARD:
		return _poly.nextnode(node());
	case BACKWARD:
		return  _poly.prevnode(node());
	default:
		assert(0);
	}
	return 0;	// Should not be reached
}

const PolyNode *
DirPolyIter::prevnode() const
{
	switch(dir())
	{case FORWARD:
		return _poly.prevnode(node());
	case BACKWARD:
		return  _poly.nextnode(node());
	default:
		assert(0);
	}
	return 0;	// Should not be reached
}

EdgeState
DirPolyIter::edgestate() const
{
	if (dir() == FORWARD)
		return node()->edgestate();
		
	return nextnode()->edgestate();
}

#ifdef notdef
int
Poly::intersect_table( int hor, Vec &intersection_table, double h )
{
	ConstPolyIter	iter(*this);
	iter();
	BoundingBox	bbox(iter.point());
	while(iter())
		bbox.add(iter.point());

	Point	left( hor ? bbox.xmin()-1 : 0, hor ? 0 : bbox.ymin()-1 ), 
			right( hor ? bbox.xmax()+1 : 0, hor ? 0 : bbox.ymax()+1 );
	
	if (hor)
		left.y() = right.y() = h;
	else
		left.x() = right.x() = h;

	int		nr_inters;
	
	if (hor)
		create_intersection_table(*this, left, right, 
						nr_inters, intersection_table, x, y );
	else
		create_intersection_table(*this, left, right, 
						nr_inters, intersection_table, y, x );
	
	return nr_inters;
}
#endif
