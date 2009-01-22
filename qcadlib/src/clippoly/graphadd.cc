static const char rcs_id[] = "$Header: /cvsroot/clippoly/clippoly/graphadd.cc,v 1.5 2005/02/28 17:21:12 klamer Exp $";

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

// $Log: graphadd.cc,v $
// Revision 1.5  2005/02/28 17:21:12  klamer
// Changed to have g++ 3.2.3 run silently using g++ -ansi -pedantic -Wall -Wno-unused -Wno-reorder.
// Change use of (libg++) String to ANSI C++ string.
//
// Revision 1.1  1994/01/04  12:55:37  klamer
// Initial revision
//
// Revision 1.6  1993/01/18  16:19:46  klamer
// Fixed bug in swapping d1 and d2.
//
// Revision 1.5  1992/10/20  10:54:07  klamer
// Made comparisons accurate.
// Made in points const references.
//
// Revision 1.4  1992/09/16  15:54:07  klamer
// Fixed bug for parallel lines...
//
// Revision 1.3  1992/09/11  14:51:38  klamer
// Numerical more stable algorithm used.
//
// Revision 1.2  1992/09/09  15:49:10  klamer
// split C and C++ parts.
//
//
// 				     GRAPHADD.CC
//
//
// author        : G.H.J. Hilhorst
//
// created       : 16-04-1992
// last modified : 17-08-1992
//

#include	<assert.h>

#include "graphadd.h"
#include <graphmat++.h>

static const char h_rcs_id[] = "$Header";

#define Dabs(x) (((x)<0) ? (-(x)):(x))

#ifndef EPSILON
#define EPSILON          1e-10
#endif

#ifdef notdef
#define Pos_cmp(pos1,pos2) (Dabs((pos1)-(pos2))<EPSILON)
#define	ChkZero(x)		(Dabs(x) < EPSILON)
#else
#define Pos_cmp(pos1,pos2)	((pos1) == (pos2))
#define	ChkZero(x)		(x == 0.0)
#endif

inline int
  operator==( const hvec2_t &h1, const hvec2_t &h2 )
{
  return (v_x(h1) == v_x(h2)) && (v_y(h1) == v_y(h2));
}

inline int
  operator!=( const hvec2_t &h1, const hvec2_t &h2 )
{
  return (v_x(h1) != v_x(h2)) || (v_y(h1) != v_y(h2));
}

inline double
  max(double x, double y)
{
  return x > y ? x : y;
}

inline double
  min(double x, double y)
{
  return x < y ? x : y;
}

static 
#ifdef __GNUG__
inline double
m_len( const hvec2_t &v )
#else
double m_len( const hvec2_t v )
#endif
{
	if (v_x(v) > 0)
		return len( v );
	if (v_x(v) < 0)
		return -len( v );
	if (v_y(v) > 0)
		return len(v);
	return -len(v);
}

static inline double
  inp_ort( const hvec2_t &a, const hvec2_t &b )
{
  return v_x(a) * v_y(b) - v_y(a) * v_x(b);
}

static void	
  recursive_intersection( const hvec2_t &p1, const hvec2_t &p2, 
			 const hvec2_t & q1, const hvec2_t &q2, hvec2_t &ret )
{
  // Find intersection point of p1-p2 and q1-q2 by iteratively
  // taking the middle of p1-p2
  
  hvec2_t	q = q2 - q1;
  
  if (len(q) < len(p2 - p1))
  {
    recursive_intersection(q1,q2,p1,p2,ret);
    return;
  }
  
  hvec2_t	s1 = p1 - q1, s2 = p2 - q1;
  hvec2_t	m = (s1 + s2) / 2.0;
  
  double	inp = inp_ort( q, s1 );
  
  if (inp > 0)
  {
      hvec2_t	tmp = s1;
      s1 = s2;
      s2 = tmp;
  }

  while ((m != s1) && (m != s2))
  {
    assert(inp_ort(q,s1) <= 0);
    assert(inp_ort(q,s2) >= 0);
#ifdef notdef
    hvec2_t	mp = m + q1 - p1, q1p = q1 - p1, q2p = q2 - p1;
    assert(inp_ort(mp,q1p) * inp_ort(mp,q2p) <= 0);
    mp = m + q1 - p2, q1p = q1 - p2, q2p = q2 - p2;
    assert(inp_ort(mp,q1p) * inp_ort(mp,q2p) <= 0);
#else
    // assert(len(m) <= len(q2 - q1));
    // assert(len(m+q1 - q2) <= len(q2 - q1));
#endif
    if (inp_ort(q,m) <= 0)
      s1 = m;
    else
      s2 = m;
    
    m = (s1 + s2) / 2.0;
  }
    assert(len(m) <= len(q2 - q1));
    assert(len(m+q1 - q2) <= len(q2 - q1));
  
  ret = m + q1;
  
  return;
}

int v_inters2(
	const hvec2_t &p1,
	const hvec2_t &p2,
	const hvec2_t &q1,
	const hvec2_t &q2,
	hvec2_t *S1,
	hvec2_t *S2
	)
/*******************************************************************
*
*  procedure that calculates the intersection point of point pair p
*            and point pair q. It returns if they hit each other and
*            the position of the hit(s) (S1 (and S2))
*
*******************************************************************/
{
	double rpx, rpy, rqx, rqy, t, deel, c1, c2, d1, d2, h;
	hvec2_t // normal,
		hv,hp1,hq1,hp2,hq2;

	if (max(v_x(p1),v_x(p2)) < min(v_x(q1),v_x(q2)))
	  return 0;
	if (max(v_x(q1),v_x(q2)) < min(v_x(p1),v_x(p2)))
	  return 0;
	
	rpx=v_x(p2)-v_x(p1);
	rpy=v_y(p2)-v_y(p1);
	rqx=v_x(q2)-v_x(q1);
	rqy=v_y(q2)-v_y(q1);

	deel=rpx*rqy-rpy*rqx;
// every value below EPSILON is considered as being 0. Hence, we do not intro-
// duce numerical inaccuracies
	// if (deel == 0) // 
	//if(Dabs(deel)<EPSILON)	/* parallel */
	if(ChkZero(deel))	/* parallel */
	{
#ifdef notdef
	// This might fail if the intersection point is far away!

	  if(rpy!=0)
	  {
	    if(rqy!=0)
	    {	// Check intersection points on X axis of lines
	     if(!Pos_cmp(v_x(p1)-v_y(p1)*rpx/rpy,v_x(q1)-v_y(q1)*rqx/rqy))
		 return(0);
	    } else if(!Pos_cmp(v_x(p1),v_x(q1)+(v_y(p1)-v_y(q1))/rqy*rqx))
		 return(0);
	  } else
	  { /* rpy=0 */
	    if(rpx!=0)
	    {
	      if(Pos_cmp(v_y(p1)-v_x(p1)*rpy/rpx,v_y(q1)-v_x(q1)*rqy/rqx))
		return(0);
	    } else if(!Pos_cmp(v_y(p1),v_y(q1)+(v_x(p1)-v_x(q1))/rqx*rqy))
		return(0);
	  }
#else
		// Check too see whether p1-p2 and q1-q2 are on the same line

		hvec2_t	q1p1, q1q2;

		vv_sub2( &q1, &p1, &q1p1 );
		vv_sub2( &q1, &q2, &q1q2 );

		// double inpr = vv_inprod2( &q1p1, &q1q2 );
		double	inpr = v_x(q1p1)*v_y(q1q2) - v_y(q1p1) * v_x(q1q2);

		// If this product is not zero then p1 is not on q1-q2!
		// if (inpr != 0) // 
		//if (!(Dabs(inpr)<EPSILON))	
		if (!ChkZero(inpr))
			return 0;
#endif

#ifdef notdef
		// This will fail if the origin is on (or close to) the line!

		vv_sub2(&p2, &p1, &normal);
		c1= vv_inprod2(&normal, &p1);  /* assume W=0 */
		c2= vv_inprod2(&normal, &p2);
		d1= vv_inprod2(&normal, &q1);
		d2= vv_inprod2(&normal, &q2);
#else
		c1 = 0;		// m_len(p1 - p1)
		c2 = m_len(p1 - p2);
		d1 = m_len(p1 - q1);
		d2 = m_len(p1 - q2);
#endif
  
/* Sorting the independent points from small to large: */
		v_cpy2(&p1,&hp1);
		v_cpy2(&p2,&hp2);
		v_cpy2(&q1,&hq1);
		v_cpy2(&q2,&hq2);
		if (c1>c2)
		{		/* hv and h are used as help-variable. */
			v_cpy2(&hp1,&hv);
			v_cpy2(&hp2,&hp1);
			v_cpy2(&hv,&hp2);
			h=c1;		 c1=c2;		  c2=h;
		}
		if (d1>d2)
		{
			v_cpy2(&hq1,&hv);
			v_cpy2(&hq2,&hq1);
			v_cpy2(&hv,&hq2);
			h=d1;		 d1=d2;		  d2=h;
		}

/* Now the line-pieces are compared: */

		if (c1<d1)
		{
			if (c2<d1) return 0;
			if (c2<d2)	{ v_cpy2(&hp2,S1); v_cpy2(&hq1,S2); }
			else		{ v_cpy2(&hq1,S1); v_cpy2(&hq2,S2); };
		}
 		else
    		{
			if (c1>d2) return 0;
     	 		if (c2<d2)	{ v_cpy2(&hp1,S1); v_cpy2(&hp2,S2); }
     	 		else	        { v_cpy2(&hp1,S1); v_cpy2(&hq2,S2); };
		}

		if((v_x(*S1)==v_x(*S2)) && (v_y(*S1)==v_y(*S2))) return(1);
		else return(2);
	}
	else	/* not parallel */
	{
/*
 * We have the lines:
 * l1: p1 + s(p2 - p1)
 * l2: q1 + t(q2 - q1)
 * And we want to know the intersection point.
 * Calculate t:
 * p1 + s(p2-p1) = q1 + t(q2-q1)
 * which is similar to the two equations:
 * p1x + s * rpx = q1x + t * rqx
 * p1y + s * rpy = q1y + t * rqy
 * Multiplying these by rpy resp. rpx gives:
 * rpy * p1x + s * rpx * rpy = rpy * q1x + t * rpy * rqx
 * rpx * p1y + s * rpx * rpy = rpx * q1y + t * rpx * rqy
 * Subtracting these gives:
 * rpy * p1x - rpx * p1y = rpy * q1x - rpx * q1y + t * ( rpy * rqx - rpx * rqy )
 * So t can be isolated:
 * t = (rpy * ( p1x - q1x ) + rpx * ( - p1y + q1y )) / ( rpy * rqx - rpx * rqy )
 * and deel = rpx * rqy - rpy * rqx
 */
	  	if ((q1 == p1) || (q1 == p2))
		  *S1 = q1;
		else if ((q2 == p1) || (q2 == p2))
		  *S1 = q2;
		else {
		  t=-(rpy*(-v_x(q1)+v_x(p1))+rpx*(v_y(q1)-v_y(p1)))/deel;
		  v_x(*S1) = v_x(q1)+t*rqx;
		  v_y(*S1) = v_y(q1)+t*rqy;
	  	  v_w(*S1) = 1;
		}

#ifdef notdef
		// Say that *S1 equals one of the points if the relative distance is smaller
		// than EPSILON
		double	l_p = len(p1 - p2);
		if (EPSILON > len(*S1 - p2) / l_p)
		  *S1 = p2;
		else if (EPSILON > len(*S1 - p1) / l_p)
		  *S1 = p1;
		else {
		  double	l_q = len(q1 - q2);
		  if (EPSILON > len(*S1 - q2) / l_q)
		  *S1 = q2;
		else if (EPSILON > len(*S1 - q1) / l_q)
		  *S1 = q1;
		}
#endif
	
/*
 * The intersection point is valid if it is
 * 1) on q1-q2 --> t >= 0 && t <= 1
 * 2) on p1-p2 --> p1 must be on the other side of q1-q2 as p2
 *    This is so if the difference of the x coordinate of p1-s1 has the
 *    opposite sign as the x coordinate of p2-s2. So the multiplication of
 *    these two must be negative. This might fail if p1-p2 is a vertical line;
 *    this can be solved by adding the same product for the y coordinates
 */
#ifdef notdef
		return((t>=0) && (t<=1) &&
			((v_x(*S1)-v_x(p1))*(v_x(*S1)-v_x(p2))+
			 (v_y(*S1)-v_y(p1))*(v_y(*S1)-v_y(p2))<=0) );
#else
#ifdef notdef
		int	condition = ((t>=0) && (t<=1) &&
			((v_x(*S1)-v_x(p1))*(v_x(*S1)-v_x(p2))+
			 (v_y(*S1)-v_y(p1))*(v_y(*S1)-v_y(p2))<=0) );
#endif
		
		// Implement an other way of checking whether the calculated point is valid.
		// This is done using the inproduct on the original points.
		hvec2_t	p = p2 - p1, pq1 = q1 - p1, pq2 = q2 - p1;
		double	inp1 = v_x(p) * v_y(pq1) - v_y(p) * v_x(pq1),
			inp2 = v_x(p) * v_y(pq2) - v_y(p) * v_x(pq2);
		int	c1 = inp1 * inp2 <= 0;

		hvec2_t	q = q2 - q1, qp1 = p1 - q1, qp2 = p2 - q1;
		double	inp3 = v_x(q) * v_y(qp1) - v_y(q) * v_x(qp1),
			inp4 = v_x(q) * v_y(qp2) - v_y(q) * v_x(qp2);
		int	c2 = inp3 * inp4 <= 0;

		//if (c1 && c2 && 0)
		{
		  // Say that *S1 equals one of the points if the relative distance is smaller
		  // than EPSILON
		  double	l_q = len(q1 - q2);
		  double	l_p = len(p1 - p2);
		  if (EPSILON > len(*S1 - p2) / l_p)
		    { *S1 = p2; c2 = 2; }
		  else if (EPSILON > len(*S1 - p1) / l_p)
		    { *S1 = p1; c2 = 2; }
		  else {
		    // double	l_q = len(q1 - q2);
		    if (EPSILON > len(*S1 - q2) / l_q)
		      { *S1 = q2; c1 = 2; }
		    else if (EPSILON > len(*S1 - q1) / l_q)
		      { *S1 = q1; c1 = 2; }
		  }
		}
		
		hvec2_t	s = *S1 - p1;
		double	inp1s = v_x(s) * v_y(pq1) - v_y(s) * v_x(pq1),
			inp2s = v_x(s) * v_y(pq2) - v_y(s) * v_x(pq2);
		int	c1s = (*S1 == p1) ? -1 : inp1s * inp2s <= 0;

		hvec2_t	qs = *S1 - q1;
		double	inp3s = v_x(qs) * v_y(qp1) - v_y(qs) * v_x(qp1),
			inp4s = v_x(qs) * v_y(qp2) - v_y(qs) * v_x(qp2);
		int	c2s = (*S1 == q1) ? -1 : inp3s * inp4s <= 0;

		// Roundig errors might make the statements below untrue
		int	failed = 0;
		if (!((c1 == 0) || (c2 == 0) || (c1s == (c1 != 0)) || (c1s == -1)))
		  failed = 1;
		else if (!((c1 == 0) || (c2 == 0) || (c2s == (c2 != 0)) || (c2s == -1)))
		  failed = 2;
		else if (c1 && c2 && (len(*S1 - q1) > len(q2 - q1)))
		  failed = 3;
		else if (c1 && c2 && (len(*S1 - q2) > len(q2 - q1)))
		  failed = 4;
		else if (c1 && c2 && (len(*S1 - p1) > len(p2 - p1)))
		  failed = 5;
		else if (c1 && c2 && (len(*S1 - p2) > len(p2 - p1)))
		  failed = 6;
		if ((failed >= 3) && (c1 == 2 || c2 == 2))
		{
		  failed = -1; c1 = c2 = 0;
	 	}
		if (failed > 0)
		  recursive_intersection(p1, p2, q1, q2, *S1);
#ifdef notdef
		assert((c1 && c2) == condition);
		
		return condition;
#else
		return (c1 && c2);
#endif
#endif
	}
}



