//    tutvis library

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

#ifndef GRAPHMATPLUSPLUS_H
#define GRAPHMATPLUSPLUS_H	"$Header: /cvsroot/clippoly/clippoly/graphmat++.h,v 1.5 2005/02/28 17:21:12 klamer Exp $"

// $Log: graphmat++.h,v $
// Revision 1.5  2005/02/28 17:21:12  klamer
// Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
// Change use of (libg++) String to ANSI C++ string.
//
// Revision 1.5  1993/05/13  12:50:59  klamer
// fixed v2 -= v2 to use vv_sub2 instead of vv_add2
// added -v3, v3 * s, s * v3, v3 + v3, v3 -= v3
//
// Revision 1.4  1993/01/18  16:23:47  klamer
// Added m3 * m3, m3 * v3 and v3 - v3.
//
// Revision 1.3  1992/10/20  11:02:52  klamer
// Added operator versions of:
// double * hvec2
// hvec2 / double
// hvec2 += hvec2
// hvec2 -= hvec2
// hvec2 *=double
// hvec2 /= double
//

#ifndef GRAPHMAT_INCLUDE
#include	<graphmat.h>
#endif

#ifdef __GNUG__
#pragma	interface
#endif

inline hvec2_t
operator-( const hvec2_t &h )
{
	hvec2_t	ret;

	v_fill2(-v_x(h),-v_y(h),v_w(h), &ret);

	return ret;
}

inline hvec2_t
operator-( const hvec2_t &l,  const hvec2_t &r )
{
	hvec2_t	ret;

	vv_sub2( &l, &r, &ret );

	return ret;
}

inline hvec2_t
operator+(  const hvec2_t &l,  const hvec2_t &r )
{
	hvec2_t	ret;

	vv_add2( &l, &r, &ret );

	return ret;
}

inline hvec2_t
operator*( const hmat2_t &m, const hvec2_t &v )
{
	hvec2_t	ret;

	mv_mul2( &m, &v, &ret );

	return ret;
}

inline hvec2_t
operator*( double s, const hvec2_t &v )
{
	hvec2_t	ret;

	sv_mul2( s, &v, &ret );

	return ret;
}

inline hvec2_t 
operator/( const hvec2_t &v, double div )
{
	hvec2_t	ret;
	
	sv_mul2( 1.0/div, &v, &ret );

	return ret;
}

inline hvec2_t const &
operator+=( hvec2_t &v, const hvec2_t &add )
{
	vv_add2( &v, &add, &v );

	return v;
}

inline hvec2_t const &
operator-=( hvec2_t &v, const hvec2_t &sub )
{
	vv_sub2( &v, &sub, &v );

	return v;
}

inline hvec2_t const &
operator*=( hvec2_t &v, double mul )
{
	sv_mul2( mul, &v, &v );

	return v;
}

inline hvec2_t const &
operator/=( hvec2_t &v, double div )
{
	sv_mul2( 1.0/div, &v, &v );

	return v;
}

inline double
len(  const hvec2_t &v )
{
	return v_len2( &v );
}

inline hvec3_t
operator-( const hvec3_t &h )
{
	hvec3_t	ret;

	v_fill3(-v_x(h),-v_y(h), -v_z(h), v_w(h), &ret);

	return ret;
}

inline hmat3_t
operator*( const hmat3_t &l, const hmat3_t &r )
{
	hmat3_t	res;
	
	mm_mul3(&l, &r, &res);
	
	return res;
}

inline hvec3_t
operator*( const hmat3_t &m, const hvec3_t &v )
{
	hvec3_t	res;
	
	mv_mul3(&m, &v, &res);
	
	return res;
}

inline hvec3_t
operator-( const hvec3_t &l, const hvec3_t &r )
{
	hvec3_t	res;
	
	vv_sub3( &l, &r, &res );
	
	return res;
}

inline hvec3_t
operator*( const hvec3_t &v, double s )
{
        hvec3_t ret;

        sv_mul3( s, &v, &ret );

        return ret;
}


inline hvec3_t
operator*( double s, const hvec3_t &v )
{
        hvec3_t ret;

        sv_mul3( s, &v, &ret );

        return ret;
}

inline hvec3_t
operator+(  const hvec3_t &l,  const hvec3_t &r )
{
	hvec3_t	ret;

	vv_add3( &l, &r, &ret );

	return ret;
}

inline hvec3_t const &
operator-=( hvec3_t &v, const hvec3_t &sub )
{
	vv_sub3( &v, &sub, &v );

	return v;
}

#endif

