#ifndef	PRIMITIVES_H
#define	PRIMITIVES_H	"$Header: /cvsroot/clippoly/clippoly/primitives.h,v 1.5 2005/02/28 17:21:12 klamer Exp $"

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

// $Log: primitives.h,v $
// Revision 1.5  2005/02/28 17:21:12  klamer
// Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
// Change use of (libg++) String to ANSI C++ string.
//
// Revision 1.1  1993/10/27  14:43:55  klamer
// Initial revision
//
// Revision 1.1  1993/10/27  14:43:55  klamer
// Initial revision
//
// Revision 1.1  1992/12/07  10:46:35  klamer
// Initial revision
//

#ifdef __GNUG__
#pragma interface
#endif

#include <cmath>
#include <cassert>
#ifndef GRAPHMAT_INCLUDE
#include	<graphmat.h>
#endif

class Point
{
	double	_x, _y;
	
public:
        Point():_x(0), _y(0)
		{ }
	Point( double x, double y )
		: _x(x), _y(y)
		{ }
	Point( const hvec2_t &h )
		: _x(v_x(h)), _y(v_y(h))
		{ }

	double	x() const
		{ return _x; }
	double	&x()
		{ return _x; }
	double	y() const
		{ return _y; }
	double	&y()
		{ return _y; }
			
	// operator hvec2_t();	// Does crash g++ 2.2.1 when used :-(
	hvec2_t	hvec() const
//		{ hvec2_t res; v_fill2( x(), y(), 1.0, &res ); return res; }
		{ hvec2_t res; v_x(res) = x(); v_y(res) = y(); v_w(res) = 1;
			return res; }
	const Point &operator=( const hvec2_t &copy )
		{ _x = v_x(copy); _y = v_y( copy); return *this; }
	const Point &operator=( const Point &copy )
		{ _x = copy.x(); _y = copy.y(); return *this; }
};

inline Point	
operator+( const Point &p1, const Point &p2 )
{
	return Point( p1.x() + p2.x(), p1.y() + p2.y() );
}
	
inline Point	
operator-( const Point &p1, const Point &p2 )
{
	return Point( p1.x() - p2.x(), p1.y() - p2.y() );
}

inline double
len( const Point p )
{
	return sqrt( p.x() * p.x() + p.y() * p.y() );
}
 
inline Point	
operator/( const Point &p, double div )
{
	return Point( p.x() / div, p.y() / div );
}

inline int
operator==( const Point &p1, const Point &p2 )
{
	return (p1.x() == p2.x()) && (p1.y() == p2.y());
}

inline int
operator!=( const Point &p1, const Point &p2 )
{
	return (p1.x() != p2.x()) || (p1.y() != p2.y());
}

inline int
operator<( const Point &p1, const Point &p2 )
{
	return (p1.y() < p2.y()) || ((p1.y() == p2.y()) && (p1.x() < p2.x()));
}

inline int
operator>( const Point &p1, const Point &p2 )
{
	return (p1.y() > p2.y()) || ((p1.y() == p2.y()) && (p1.x() > p2.x()));
}

double	angle( const Point &p1, const Point &p2, const Point &p3 );

inline double
atan( const Point &p )
{
	assert( (p.x() != 0) || (p.y() != 0) );
	return atan2( p.y(), p.x() );
}

inline Point 
point(const hvec3_t &p)
{
	hvec3_t	pn;
	v_homo3( &p, &pn );
	
	return Point( v_x(pn), v_y(pn) );
}

class PointList
{
	friend class PointListIter;
	
	int	len, cur;
	Point	*points;

	static const int	def_len; //	= 16;
	// Make copy constructor and assignment unusable
	PointList( PointList &copy );
	PointList &operator=( const PointList &copy );
		
public:
	PointList( int nr = def_len );
	~PointList();
	
	void	add( const Point &add );
};

class PointListIter
{
	const PointList	&pl;
	int				cnt;
	
public:
	PointListIter( const PointList &list )
		: pl( list ), cnt( -1 )
		{ }
	
	int	operator() ()
		{ if (++cnt < pl.cur) return 1; else return 0; }
	
	const Point	&point() const
		{ return pl.points[cnt]; }
};

class Edge
{
	Point	point1, point2;
	// int		shared;
	
public:
	Edge( const Point &p1, const Point &p2 ) //, int share = -1 )
		: point1( p1 ), point2( p2 ) //, shared( share )
		{ }
	Point	middle() const
		{ return (point1 + point2) / 2.0; }
	// void	set_shared( int val )
	//	{ shared = val; }
	
	const Point	&p1() const
		{ return point1; } 
	const Point 	&p2() const
		{ return point2; }	
};

#endif	/* PRIMITIVES_H */
