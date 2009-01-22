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
 *	$Log: err.h,v $
 *	Revision 1.6  2005/02/28 21:12:05  klamer
 *	Made changes such that gcc 3.4.2 compiles silent with -ansi -pedantic -Wall.
 *	
 *	Revision 1.3  2000/11/10 07:41:07  kscp1
 *	Added printf style attributes to warning, error and fatal as supported by GCC
 *
 *	Revision 1.2  1996/09/10 13:04:56  kscp1
 *	*** empty log message ***
 *
 * Revision 1.1  1994/06/07  14:30:35  schutte
 * Initial revision
 *
 * Revision 1.5  1992/06/01  15:37:48  klamer
 * Made prototypes gcc compatible.
 *
 * Revision 1.4  1991/11/15  14:55:03  klamer
 * Merged with /usr/local/include/err.h
 *
 * Revision 1.3  1991/11/15  14:52:04  klamer
 * Made K&R C compatible as well.
 *
 * Revision 1.2  1991/10/11  09:55:58  klamer
 * added prototype for warning.
 *
 * Revision 1.1  1991/09/16  12:59:45  klamer
 * Initial revision
 *
 */

#ifndef	_ERROR
#define	_ERROR	"$Id: err.h,v 1.6 2005/02/28 21:12:05 klamer Exp $"

#ifdef __cplusplus
#define	EC	extern "C"
#define	P(x)	x
#define	CDOTS	...
#else
#define	EC	/* nothing */
#ifdef __STDC__
#define	P(x)	x
#define	CDOTS	, ...
#else
#define	P(x)	(/* x */)
#endif
#endif
#ifdef __GNUG__ /* is it GCC or G++ ? */
#define NORETURN_PRINTF __attribute__ ((noreturn, format(printf, 1, 2)))
#define PRINTF __attribute__ ((format(printf, 1, 2)))
#else
#define NORETURN_PRINTF
#define PRINTF
#endif

extern int	err_nr_errors, err_nr_warnings;
/* what is the type of a pointer to a jmp_buf? On sun4 int * will do */
extern int	*err_warning_jmpbuf, *err_error_jmpbuf, *err_fatal_jmpbuf;

EC void	warning P((const char *fmt CDOTS )) PRINTF;
EC void	error P((const char *fmt CDOTS )) PRINTF;
EC void	fatal P((const char *fmt CDOTS )) NORETURN_PRINTF;

#undef PRINTF
#undef NORETURN_PRINTF
#undef EC
#undef P
#undef CDOTS

#endif	/* _ERROR */

