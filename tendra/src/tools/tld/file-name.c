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


/**** file-name.c --- File name manipulation routines.
 *
 ** Author: Steve Folkes <smf@hermes.mod.uk>
 *
 **** Commentary:
 *
 * This file implements the file name manipulation facility specified in the
 * file "file-name.h".  See that file for more details.
 *
 **** Change Log:*/

/****************************************************************************/

#include "file-name.h"
#include "cstring.h"

/*--------------------------------------------------------------------------*/

CStringP
file_name_basename(CStringP path)
{
    CStringP last = cstring_find_reverse (path, '/');

    if (last) {
		return (cstring_duplicate (last + 1));
    } else {
		return (cstring_duplicate (path));
    }
}

CStringP
file_name_dirname(CStringP path)
{
    CStringP last = cstring_find_reverse (path, '/');

    if (last) {
		return (cstring_duplicate_prefix (path, (unsigned) (last - path)));
    } else {
		return (cstring_duplicate (path));
    }
}

CStringP
file_name_expand(CStringP dir, CStringP name,
				 CStringP suffix)
{
    unsigned dir_length  = cstring_length (dir);
    unsigned name_length = cstring_length (name);
    unsigned suf_length  = (suffix ? (cstring_length (suffix)) : 0);
    unsigned suf_extra   = (unsigned) (suffix ? 1 : 0);
    unsigned extra;
    unsigned length;
    CStringP path;
    CStringP tmp;

    if ((dir_length > 0) && (dir [dir_length - 1] != '/')) {
		extra = 1;
    } else {
		extra = 0;
    }
    length = (dir_length + extra + name_length + suf_extra + suf_length + 1);
    path   = ALLOCATE_VECTOR (char, length);
    tmp    = path;
    if (dir_length > 0) {
		(void) memcpy ((GenericP) tmp, (GenericP) dir, (SizeT) dir_length);
		tmp += dir_length;
		if (dir [dir_length - 1] != '/') {
			tmp [0] = '/';
			tmp ++;
		}
    }
    (void) memcpy ((GenericP) tmp, (GenericP) name, (SizeT) name_length);
    tmp += name_length;
    if (suffix) {
		tmp [0] = '.';
		tmp ++;
		(void) memcpy ((GenericP) tmp, (GenericP) suffix, (SizeT) suf_length);
		tmp += suf_length;
    }
    tmp [0] = '\0';
    return (path);
}

BoolT
file_name_is_basename(CStringP path)
{
    return (!cstring_contains (path, '/'));
}

BoolT
file_name_is_absolute(CStringP path)
{
    return (path [0] == '/');
}

void
file_name_populate(CStringP path)
{
#ifdef FS_MKDIR
    CStringP new_path = cstring_duplicate (path);
    CStringP tmp      = cstring_find (new_path, '/');

    if (tmp) {
		do {
			*tmp = '\0';
			(void) mkdir (new_path, 0755);
			*tmp = '/';
		} while ((tmp = cstring_find (tmp + 1, '/')) != NIL (CStringP));
    }
    DEALLOCATE (new_path);
#else
    UNUSED (path);
#endif /* defined (FS_MKDIR) */
}
