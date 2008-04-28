/*
 * Copyright (c) 2002-2005 The TenDRA Project <http://www.tendra.org/>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of The TenDRA Project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific, prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */
/*
    		 Crown Copyright (c) 1997

    This TenDRA(r) Computer Program is subject to Copyright
    owned by the United Kingdom Secretary of State for Defence
    acting through the Defence Evaluation and Research Agency
    (DERA).  It is made available to Recipients with a
    royalty-free licence for its use, reproduction, transfer
    to other parties and amendment for any purpose not excluding
    product development provided that any such use et cetera
    shall be deemed to be acceptance of the following conditions:-

        (1) Its Recipients shall ensure that this Notice is
        reproduced upon any copies or amended versions of it;

        (2) Any amended version of it shall be clearly marked to
        show both the nature of and the organisation responsible
        for the relevant amendment or amendments;

        (3) Its onward transfer from a recipient to another
        party shall be deemed to be that party's acceptance of
        these conditions;

        (4) DERA gives no warranty or assurance as to its
        quality or suitability for any purpose and DERA accepts
        no liability whatsoever in relation to any use to which
        it may be put.
*/


#include "config.h"
#include "object.h"
#include "hash.h"
#include "name.h"
#include "type.h"
#include "utility.h"
#include "makefile.h"


/*
    IS A FILE NEEDED IN THE MAKEFILE?

    This routine tests whether a file with information i is needed in the
    makefile for the given api.
*/

static
boolean need_info(info *i, char *api)
{
    if (restrict_depth && strcmp(api, i->api) != 0) return(0);
    return(i->implemented && i->tokens && i->src);
}


/*
    PRINT A MAKEFILE

    This routine prints a makefile to build a TDF library for the API api
    from the list of files f.  There are two forms of the output, indicated
    by whole.
*/

void
print_makefile(char *api, hash_elem *f)
{
    char *nm;
    FILE *output;
    hash_elem *e;
    char *api2 = hack_name(api, "_Aa0");

    /* Open output file */
    nm = MAKEFILE;
    nm = string_printf(nm, output_src_dir, api, api2);
    if (verbose > 1) IGNORE printf("Creating %s ...", nm);
    create_dir(nm);
    output = fopen(nm, "w");
    if (output == NULL) {
	error(ERR_SERIOUS, "Can't open output file, %s", nm);
	return;
    }
    fprintf(output, "APILIB =\t%s.tl\n\n", api);
	fputs("APIOBJS =\t", output);
	for ( e = f ; e != NULL ; e = e->next ) {
		info *i = e->obj->u.u_info ;
		if ( need_info ( i, api ) ) {
			int m;
			strcpy(buffer, basename(i->src));
			m = strlen(buffer) - 1;
			buffer[m] = 'j';
			fprintf(output, "\\\n\t\t%s.api/%s", api, basename(buffer));
		}
	}
	fputs("\n", output);
    /* End of makefile */
    IGNORE fclose(output);
    return;
}
