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
static const char rcs_id[] = "$Header: /cvsroot/clippoly/clippoly/err.c,v 1.2 2005/02/28 21:12:05 klamer Exp $";
/*
 * $Log: err.c,v $
 * Revision 1.2  2005/02/28 21:12:05  klamer
 * Made changes such that gcc 3.4.2 compiles silent with -ansi -pedantic -Wall.
 *
 * Revision 1.2  2003/11/11 12:57:38  kscp1
 * Made changes such that linux/gcc-3.3 compiles without warnings.
 &
 * Revision 1.2  2000/11/10 08:05:43  kscp1
 * ANSI C version
 *
 * Revision 1.1  1994/06/07  14:30:35  schutte
 * Initial revision
 *
 *
 *	error.c				Klamer Schutte 25/7/90
 *
 *	handle errors in a consistent way.
 */

#include	<stdio.h>
#include	<stdarg.h>
#include	<setjmp.h>
#include <stdlib.h>

#include "err.h"

int	err_nr_errors, err_nr_warnings;
/* what is the type of a pointer to a jmp_buf? On sun4 int * will do */
int	*err_warning_jmpbuf, *err_error_jmpbuf, *err_fatal_jmpbuf;

void
/*VARARGS1*/
warning( const char *fmt, ... )
{
	va_list	args;

	va_start( args, fmt );
	vfprintf(stderr, fmt, args );
	va_end(args );

	err_nr_warnings++;

	if (err_warning_jmpbuf != NULL)
		longjmp( (void *) err_warning_jmpbuf, 1 );
}

void
/*VARARGS1*/
error( const char *fmt, ... )
{
	va_list	args;

	va_start( args, fmt );
	vfprintf(stderr, fmt, args );
	va_end(args );

	err_nr_errors++;

	if (err_error_jmpbuf != NULL)
		longjmp( (void *) err_error_jmpbuf, 1 );
}	

void
/*VARARGS2*/
fatal_exit_nr( int nr, const char *fmt, ... )
{
	va_list	args;

	va_start( args, fmt );
	vfprintf(stderr, fmt, args );
	va_end(args );

	if (err_fatal_jmpbuf != NULL)
		longjmp( (void *) err_fatal_jmpbuf, 1 );

	exit( nr );
}	

void
/*VARARGS1*/
fatal( const char *fmt, ... )
{
	va_list	args;

	va_start( args, fmt );
	vfprintf(stderr, fmt, args );
	va_end(args );

	if (err_fatal_jmpbuf != NULL)
		longjmp(  (void *) err_fatal_jmpbuf, 1 );

	exit( 1 );
}	
