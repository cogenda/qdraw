/*
 *    tutvis library

 *    Copyright (C) 1993  University of Twente

 *    klamer@mi.el.utwente.nl

 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Library General Public
 *    License as published by the Free Software Foundation; either
 *    version 2 of the License, or (at your option) any later version.

 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Library General Public License for more details.

 *    You should have received a copy of the GNU Library General Public
 *    License along with this library; if not, write to the Free
 *    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * $Log: graphadd.h,v $
 * Revision 1.7  2005/02/28 21:12:05  klamer
 * Made changes such that gcc 3.4.2 compiles silent with -ansi -pedantic -Wall.
 *
 * Revision 1.6  2005/02/28 17:26:49  klamer
 * Changed comment with $Log: graphadd.h,v $
 * Changed comment with Revision 1.7  2005/02/28 21:12:05  klamer
 * Changed comment with Made changes such that gcc 3.4.2 compiles silent with -ansi -pedantic -Wall.
 * Changed comment with to get rid of warning.
 *
 * Revision 1.5  2005/02/28 17:21:12  klamer
 * Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
 * Change use of (libg++) String to ANSI C++ string.
 *
 * Revision 1.3  1992/10/20  13:48:39  klamer
 * Made input arguments of v_inters const.
 *
// Revision 1.2  1992/09/09  15:49:10  klamer
// split C and C++ parts.
// Added ANSI C support.
//
//
// 				     GRAPHADD.H
//
//
// author        : G.H.J. Hilhorst
//
// created       : 16-04-1992
// last modified : 21-05-1992
*/

#ifndef GRAPHADD_H
#define	GRAPHADD_H	"$Header: /cvsroot/clippoly/clippoly/graphadd.h,v 1.7 2005/02/28 21:12:05 klamer Exp $"

#ifndef GRAPHMAT_INCLUDE
#include	<graphmat.h>
#endif

#define	v_print2(vec,vec_name)(fprintf(stderr,"vec2 %s:\tx=%g\t y=%g\tw=%g\n",\
	vec_name,v_x(vec),v_y(vec),v_w(vec)))

#define	v_print3(vec,vec_name)(fprintf(stderr,\
	"vec3 %s:\tx=%g\t y=%g\tz=%g\tw=%g\n",\
	vec_name,v_x(vec),v_y(vec),v_z(vec),v_w(vec)))

#define	m_print2(mat,vec_name) if(&(mat)!=NULL) fprintf(stderr, \
	"mat2 %s:\n%g\t%g\t%g\n%g\t%g\t%g\n%g\t%g\t%g\n", \
	vec_name,m_elem(mat,0,0),m_elem(mat,0,1),m_elem(mat,0,2), \
	m_elem(mat,1,0),m_elem(mat,1,1),m_elem(mat,1,2), \
	m_elem(mat,2,0),m_elem(mat,2,1),m_elem(mat,2,2))

#define	m_print3(mat,vec_name) if(&(mat)!=NULL) fprintf(stderr, \
 "mat3 %s:\n%g\t%g\t%g\t%g\n%g\t%g\t%g\t%g\n%g\t%g\t%g\t%g\n%g\t%g\t%g\t%g\n",\
     vec_name,m_elem(mat,0,0),m_elem(mat,0,1),m_elem(mat,0,2),m_elem(mat,0,3),\
	m_elem(mat,1,0),m_elem(mat,1,1),m_elem(mat,1,2),m_elem(mat,1,3), \
	m_elem(mat,2,0),m_elem(mat,2,1),m_elem(mat,2,2),m_elem(mat,2,3), \
	m_elem(mat,3,0),m_elem(mat,3,1),m_elem(mat,3,2),m_elem(mat,3,3))

#define v_scan2(vec)(scanf("%lf %lf %lf",&v_x(*vec),&v_y(*vec),&v_w(*vec)))

#define v_scan3(vec)(scanf("%lf %lf %lf %lf",&v_x(*vec),&v_y(*vec),\
	&v_z(*vec),&v_w(*vec)))

#ifdef __cplusplus

int v_inters2(
	const hvec2_t &p1,
	const hvec2_t &p2,
	const hvec2_t &q1,
	const hvec2_t &q2,
	hvec2_t *S1,
	hvec2_t *S2
	);

extern "C" {
#endif


void	v_dupl2(hvec2_t *, hvec2_t *);
void	v_dupl3(hvec3_t *, hvec3_t *);
void	m_dupl2(hmat2_t *, hmat2_t *);
void	m_dupl3(hmat3_t *, hmat3_t *);

#ifdef __cplusplus
}
#endif

#endif

