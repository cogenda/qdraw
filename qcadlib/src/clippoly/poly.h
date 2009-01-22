//    nclip: a polygon clip library

//    Copyright (C) 1993  Klamer Schutte

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

#ifndef	POLY_H
#define	POLY_H	"$Header: /cvsroot/clippoly/clippoly/poly.h,v 1.6 2005/02/28 21:12:05 klamer Exp $"

// $Log: poly.h,v $
// Revision 1.6  2005/02/28 21:12:05  klamer
// Made changes such that gcc 3.4.2 compiles silent with -ansi -pedantic -Wall.
//
// Revision 1.5  2005/02/28 17:21:12  klamer
// Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
// Change use of (libg++) String to ANSI C++ string.
//
// Revision 1.2  1994/01/04  12:55:37  klamer
// Changed PolyNode constructors.
// Added Poly::revert() member.
//
// Revision 1.1  1993/10/27  14:43:52  klamer
// Initial revision
//
// Revision 1.2  1992/12/07  13:32:35  klamer
// Deleted comments from Poly::Poly(const Point &) in-class defined
// constructor.
//
// Revision 1.1  1992/12/07  10:46:35  klamer
// Initial revision
//

#ifdef __GNUG__
#pragma interface
#endif

#include <iostream>

#ifndef PRIMITIVES_H
#include	<primitives.h>
#endif
#ifndef SET_H
#include	<set.h>
#endif
#ifndef POSADDER_H
#include	<posadder.h>
#endif

int		intersect_right( const Edge &edge, const Point &point );

enum EdgeState { Unknown, None, Shared, Inside };
// enum LogicStates;
class Vec;
class Poly;

class PolyNode
{
	friend class Poly;
	friend class PolyIter;
	friend class ConstPolyIter;
	friend std::ostream &operator<<(std::ostream &, const PolyNode &);

public:
	PolyNode	*link()
				{ return _link; }
	const PolyNode	*link() const
				{ return _link; }
	const Point	&point() const
				{ return p; }
	EdgeState	edgestate() const
				{ return _edgestate; }
	const PolyNode	&prevnode() const;
				// { return *_parent_poly->prevnode(this); }
	const PolyNode	&nextnode() const;
				// { return *_parent_poly->nextnode(this); }

private:	
	PolyNode( const PolyNode &copy );	// Don't use
	PolyNode( const PolyNode &copy, const Poly *parent );
	PolyNode( const Point &point, const Poly *parent, PolyNode *tail = 0)
    		: p( point ), next( tail ), prev(0), _link( 0 ), 
                  _parent_poly( parent ), _edgestate( ::Unknown )
		{ }
	PolyNode( const Point &point, const Poly *parent, EdgeState es )
		: p( point ), next( 0 ), prev(0), _link( 0 ), 
		  _parent_poly( parent ), _edgestate( es ) 
		{ }
	~PolyNode();
	
	void		operator=( PolyNode node );	// Don't use it!
	
	Point		p;
	PolyNode	*next, *prev;
	PolyNode	*_link;
	const Poly	*_parent_poly;
	const Poly	*parent_poly() const;
	
	EdgeState _edgestate;

};

class DirPolyIter;

class NodePEdge
{
	const PolyNode	*n1, *n2;
	friend class Set<NodePEdge>;
	friend class RSet<NodePEdge>;
	NodePEdge()
		{ }
	
public:
	NodePEdge( const PolyNode *n_1, const PolyNode *n_2 )
		: n1( n_1 ), n2( n_2 )
		{ }
	NodePEdge( const DirPolyIter &dpi );
	int		operator==( const NodePEdge &cmp ) const
			{ return (n1 == cmp.n1) && (n2 == cmp.n2); }
};

typedef Set<const PolyNode *> 		PolyNodePList;
typedef SetIter<const PolyNode *>	PolyNodePListIter;
typedef	RSet<NodePEdge>				NodePEdgeList;
typedef	RSetIter<NodePEdge>			NodePEdgeListIter;

enum Orientation { ClockWise, CounterClockWise };

class Poly
{
private:
	friend class PolyIter;
	friend class DirPolyIter;
	
	PolyNode		*list, *prev;
	Poly &operator=( const Poly &copy );		// Don't use it!
	
public:
	Poly( const Poly &copy )
		: list( new PolyNode( *copy.list, this ) ), prev( 0 )
		{ }
//	Poly( const PolyNode *copy, const Poly *parent )
//		: list( new PolyNode( copy, parent ) ), prev(0)
//		{ }
	Poly( const Point &p, const Poly *parent, EdgeState es )
		: list( new PolyNode( p, parent, es ) ), prev(0)
		{ }
	Poly( const Point &p )
		// : list( new PolyNode( &PolyNode(p), 0 ) ), prev(0)
		: list( new PolyNode( p, this ) ), prev(0)
		{ }
	~Poly()
		{ delete list; }

	void	add( const Point &p, const Poly *parent, EdgeState es );
	void	add( const Point &p )
		{	add( p, 0, ::Unknown ); }	// 0 or this ???
			
	int	has_point( const Point &point ) const;	// point inside *this?

	const PolyNode	*nextnode( const PolyNode *node ) const;
	
	Orientation		orientation() const;
	double			area() const;
	void			revert();

	void			make_prev() const;
	const PolyNode	*prevnode( const PolyNode *node ) const;
	
	const Point		&firstpoint() const
					{ return list->point(); }
	const PolyNode	*firstnode() const
					{ return list; }
					
	double			xmin() const;
	double			xmax() const;
	double			ymin() const;
	double			ymax() const;

	int			intersect_table( int hor, Vec &it, double h );
};

inline const PolyNode &
PolyNode::prevnode() const
{ 
  return *_parent_poly->prevnode(this); 
}

inline const PolyNode &
PolyNode::nextnode() const				
{ 
  return *_parent_poly->nextnode(this); 
}

typedef Set<Poly *> PolyPList;
typedef SetIter<Poly *>	PolyPListIter;

class PolyIter
{
	Poly		&poly;
	PolyNode	*cur, *app_next;
	PolyNode	*AppNext()
				{ return (app_next != 0) ? app_next : poly.list; }
	const PolyNode	*AppNext() const
				{ return (app_next != 0) ? app_next : poly.list; }
	PolyNode	*next(PolyNode *node)
				{ return (node->next != 0) ? node->next : poly.list; }
	PolyNode	*add( const Point &point, int &new_point );
	PolyNode	*add( const Point &point )
	  			{ int dummy; return add(point, dummy); }
	
public:
	PolyIter( Poly &poly );
	void		reset()
				{ app_next = poly.list; }

	int			operator() ();
	PolyNode	*node()
				{ return cur; }
	const PolyNode	*node() const
				{ return cur; }
	const PolyNode	*nextnode() const
				{ return AppNext(); }
	const PolyNode	*prevnode() const
				{ return poly.prevnode(node()); }
		
	Edge		edge() const
				{ return Edge( cur->p, AppNext()->p ); }
	static int	add_point( PolyIter &a, PolyIter &b, const Point &p );
	
	void		set_shared()
				{ cur->_edgestate = ::Shared; }
	void		set_inside()
				{ cur->_edgestate = ::Inside; }
	void		set_none()
				{ cur->_edgestate = ::None; }
};

enum IterDirection { FORWARD, BACKWARD, NONE };

class DirPolyIter
{
	const Poly		&_poly, &_linkpoly;
	const IterDirection	_dir;
	const PolyNode	*_node;
	
	IterDirection	dir() const
					{ return _dir; }
	const Poly		&linkpoly() const
					{ return _linkpoly; }
	const PolyNode	*prevnode() const;
					
public:
	DirPolyIter( const Poly &poly, const PolyNode *node,
							const Poly &link, IterDirection dir );

	// Continue on link  
	DirPolyIter( const DirPolyIter &dpi, IterDirection dir );
	
	void			next()
					{ _node = nextnode(); }
	const PolyNode	*node() const
					{ return _node; }
	const PolyNode	*nextnode() const;
	const PolyNode	*link() const
					{ return node()->link(); }
	const Point		&point() const
					{ return node()->point(); }
	const Point		&nextpoint() const
					{ return nextnode()->point(); }
	const Point		&linknextpoint() const
					{ return linkpoly().nextnode(link())->point(); }
	const Point		&linkprevpoint() const
					{ return linkpoly().prevnode(link())->point(); }
	const Poly		&poly() const
					{ return _poly; }
	EdgeState		edgestate() const;
};	

class ConstPolyIter 
{
	PolyIter		polyiter;
	const PolyNode	*prevnode() const
					{ return polyiter.prevnode(); }
	const PolyNode	*nextnode() const
					{ return polyiter.nextnode(); }
					
public:
	ConstPolyIter( const Poly &poly );

	int				operator() ()
					{ return polyiter(); }
	const PolyNode	*node() const
					{ return polyiter.node(); }
	LogicStates		parent(const Poly &poly);
	const Point		&point() const
					{ return node()->point(); }
	const Point		&prevpoint() const
					{ return prevnode()->point(); }
	const Point		&nextpoint() const
					{ return nextnode()->point(); }
	Edge			edge() const
					{ return polyiter.edge(); }
};
	
#endif	/* POLY_H */
