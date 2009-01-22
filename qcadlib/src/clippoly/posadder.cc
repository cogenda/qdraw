static const char rcs_id[] = "$Header: /cvsroot/clippoly/clippoly/posadder.cc,v 1.5 2005/02/28 17:21:12 klamer Exp $";

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

// $Log: posadder.cc,v $
// Revision 1.5  2005/02/28 17:21:12  klamer
// Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
// Change use of (libg++) String to ANSI C++ string.
//
// Revision 1.2  1994/01/04  12:55:37  klamer
// *** empty log message ***
//
// Revision 1.1  1993/10/27  14:44:10  klamer
// Initial revision
//
// Revision 1.1  1992/12/07  10:46:35  klamer
// Initial revision
//

#ifdef __GNUG__
#pragma implementation
#endif

#include	<err.h>

#include	"posadder.h"

static const char h_rcs_id[] = POSADDER_H;

void
PosAdder::set(LogicStates boolean)
{
  if (val == TrueFalse)
    return;
  
	switch(boolean)
	{case UnKnown:
		break;
   case TrueFalse:
		val = TrueFalse;
		break;
	case True:
		if (val == False)
			//error("Conflict in PosAdder::set (False, True)\n");
		  val = TrueFalse;
		else
		  val = True;
		break;
	case False:
		if (val == True)
			//error("Conflict in PosAdder::set (True, False)\n");
		  val = TrueFalse;
		else
		val = False;
		break;
	}
}
		
	
