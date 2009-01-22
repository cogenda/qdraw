#ifndef	POSADDER_H
#define	POSADDER_H	"$Header: /cvsroot/clippoly/clippoly/posadder.h,v 1.5 2005/02/28 17:21:12 klamer Exp $"

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

// $Log: posadder.h,v $
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
	
enum LogicStates	{ UnKnown, True, False, TrueFalse };

class PosAdder
{
	LogicStates		val;
public:
	PosAdder()
		: val( UnKnown )
		{ }
	void			set( LogicStates boolean );
	LogicStates		operator() () const
					{ return val; }
};

#endif	/* POSADDER_H */
