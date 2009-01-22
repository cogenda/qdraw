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

#ifndef	NCLIP_H
#define	NCLIP_H	"$Header: /cvsroot/clippoly/clippoly/nclip.h,v 1.5 2005/02/28 17:21:12 klamer Exp $"

// $Log: nclip.h,v $
// Revision 1.5  2005/02/28 17:21:12  klamer
// Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
// Change use of (libg++) String to ANSI C++ string.
//
// Revision 1.1  1993/10/27  14:43:51  klamer
// Initial revision
//
// Revision 1.1  1993/10/27  14:43:51  klamer
// Initial revision
//
// Revision 1.1  1992/12/07  10:46:35  klamer
// Initial revision
//

#ifdef _GNUG__
#pragma interface
#endif

class	Poly;
// class	PolyPList;
class	Edge;
class	DirPolyIter;
// class	PolyNodePList;
class	Point;
// class	NodePEdgeList;

void	clip_poly( const Poly &a, const Poly &b, 
			PolyPList &a_min_b, PolyPList &b_min_a, PolyPList &a_and_b );
int		intersect( const Edge &a, const Edge &b, Point &p1, Point &p2 );
int		intersect( Poly &a, Poly &b );
void	label_shared( Poly & a, const Poly & b );
void	make_poly( const Point &point, DirPolyIter &follow, 
			PolyPList &polylist, NodePEdgeList &done );
int 	make_poly( const Point &start_point, const Point &point, 
			DirPolyIter &follow, NodePEdgeList &done, 
			Poly * new_poly );
void	assign_polys( const Poly &a, const Poly &b, const PolyPList &in_list, 
			PolyPList &a_min_b, PolyPList &b_min_a, PolyPList &a_and_b );
void	poly_min_poly( const Poly &a, const Poly &b, PolyPList &a_min_b );
void	add_until( Poly *, const Point &, DirPolyIter & );

#endif	/* NCLIP_H */
