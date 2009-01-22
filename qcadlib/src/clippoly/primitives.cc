static const char rcs_id[] = "$Header: /cvsroot/clippoly/clippoly/primitives.cc,v 1.5 2005/02/28 17:21:12 klamer Exp $";

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

// $Log: primitives.cc,v $
// Revision 1.5  2005/02/28 17:21:12  klamer
// Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
// Change use of (libg++) String to ANSI C++ string.
//
// Revision 1.2  1994/01/04  12:55:37  klamer
// Loosened assertion in angle() from angle < 2 pi to angle <= 2 pi.
//
// Revision 1.1  1993/10/27  14:44:13  klamer
// Initial revision
//
// Revision 1.1  1992/12/07  10:46:35  klamer
// Initial revision
//

#ifdef __GNUG__
#pragma implementation
#endif

#include	<cassert>
#include	<cmath>

#ifndef M_PI
#define M_PI            3.14159265358979323846  /* pi */
#endif

#include	<err.h>

#include	"primitives.h"

static const char h_rcs_id[] = PRIMITIVES_H;

const int PointList::def_len = 16;

PointList::PointList( int nr )
	: len( nr ), cur( 0 ), points( new Point[ nr ] )
{
	assert( nr > 0 );
}

PointList::~PointList()
{
	delete [] points;
}

void
PointList::add( const Point &add )
{
	if (cur + 1 >= len)
		fatal("PointList::add: Array too short (%d)\n", len);
	
	points[cur] = add;
}

double
angle(const Point &p1, const Point &p2, const Point &p3)
{
	Point	d1 = p1 - p2, d2 = p3 - p2;
	
	double	res = atan( d2 ) - atan( d1 );
	
	if (res < 0)
		res += M_PI * 2;

	assert( res >= 0 );
	assert( res <= M_PI * 2 );
			
	return res;
}

