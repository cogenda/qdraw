static const char rcs_id[] = "$Header: /cvsroot/clippoly/clippoly/templates.cc,v 1.5 2005/02/28 17:21:12 klamer Exp $";

// $Log: templates.cc,v $
// Revision 1.5  2005/02/28 17:21:12  klamer
// Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
// Change use of (libg++) String to ANSI C++ string.
//
// Revision 1.1  1995/04/27  07:19:20  klamer
// Initial revision
//

#include	"set.h"
#include	"poly.h"

template class SetIter<Poly *>;
template class Set<Poly *>;
template class  RSet<NodePEdge>;
