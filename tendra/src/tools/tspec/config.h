/*
 * Copyright (c) 2002, The Tendra Project <http://www.ten15.org/>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *    		 Crown Copyright (c) 1997
 *    
 *    This TenDRA(r) Computer Program is subject to Copyright
 *    owned by the United Kingdom Secretary of State for Defence
 *    acting through the Defence Evaluation and Research Agency
 *    (DERA).  It is made available to Recipients with a
 *    royalty-free licence for its use, reproduction, transfer
 *    to other parties and amendment for any purpose not excluding
 *    product development provided that any such use et cetera
 *    shall be deemed to be acceptance of the following conditions:-
 *    
 *        (1) Its Recipients shall ensure that this Notice is
 *        reproduced upon any copies or amended versions of it;
 *    
 *        (2) Any amended version of it shall be clearly marked to
 *        show both the nature of and the organisation responsible
 *        for the relevant amendment or amendments;
 *    
 *        (3) Its onward transfer from a recipient to another
 *        party shall be deemed to be that party's acceptance of
 *        these conditions;
 *    
 *        (4) DERA gives no warranty or assurance as to its
 *        quality or suitability for any purpose and DERA accepts
 *        no liability whatsoever in relation to any use to which
 *        it may be put.
 *
 * $TenDRA$
 */


#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED


/*
 *    ANSI API SPECIFICATION
 *
 *    The API for this program is ANSI.  This defines the basic interface,
 *    and utility.c includes stdarg.h (or varargs.h).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/*
 *    COMPILER DEPENDENT MACROS
 */

#ifdef __TenDRA__
#pragma TenDRA keyword SET for set
#pragma TenDRA keyword UNUSED for discard variable
#pragma TenDRA keyword IGNORE for discard value
#pragma TenDRA keyword EXHAUSTIVE for exhaustive
#pragma TenDRA keyword REACHED for set reachable
#pragma TenDRA keyword UNREACHED for set unreachable
#define FALL_THROUGH

#elif defined ( lint ) || defined ( __lint )
#define SET(name)
#define UNUSED(name)		(name) = (name)
#define IGNORE			(void)
#define EXHAUSTIVE
#define REACHED
#define UNREACHED
#define FALL_THROUGH

#else
#define SET(name)
#define UNUSED(name)
#define IGNORE			(void)
#define EXHAUSTIVE
#define REACHED
#define UNREACHED
#define FALL_THROUGH
#endif


/*
 *    BASIC TYPES
 *
 *    These types are so ubiquitous that this is the only suitable place
 *    for them.
 */

typedef char boolean;
typedef void *pointer;


/*
 *    UTILITY MACROS
 *
 *    These macros are just a matter of programming style.
 */

#define null			NULL
#define null_str		((char *) NULL)
#define streq(X, Y)		(strcmp ((X), (Y)) == 0)
#define strneq(X, Y, Z)	(strncmp ((X), (Y), (Z)) == 0)


#endif
