#ifndef	POLY_IO_H
#define	POLY_IO_H	"$Header: /cvsroot/clippoly/clippoly/poly_io.h,v 1.5 2005/02/28 17:21:12 klamer Exp $"

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

// $Log: poly_io.h,v $
// Revision 1.5  2005/02/28 17:21:12  klamer
// Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
// Change use of (libg++) String to ANSI C++ string.
//
// Revision 1.1  1992/12/07  10:46:35  klamer
// Initial revision
//

#ifdef __GNUG__
#pragma interface
#endif

#include	<iostream>

#include <poly.h>

#define	POLY_MAGIC	"PolyMagic"

class Poly;
class Point;
class PolyNode;
union	hvec3_t;

Poly 	*read_poly( std::istream & );

std::istream &operator>>(std::istream &, Point &);
std::ostream	&operator<<(std::ostream &, const Poly &);
std::ostream	&operator<<(std::ostream &, const PolyPList &);
std::ostream	&operator<<(std::ostream &, const PolyNode &);
std::ostream	&operator<<(std::ostream &, const Point &);
std::ostream	&operator<<(std::ostream &, enum EdgeState);
std::ostream	&operator<<(std::ostream &, enum LogicStates);
std::ostream	&operator<<(std::ostream &, const hvec3_t &);

#endif	/* POLY_IO_H */
