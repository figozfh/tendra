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


/**********************************************************************
 *$Author$
 *$Date$
 *$Revision$*/
#include "config.h"
#include "release.h"
#include "util.h"
#include "namedecs.h"
#include "lex.h"
#include "includes.h"
#include "syntax.h"
#include "units.h"

#ifndef RELEASE
#define RELEASE		"unknown"
#endif

static char *pl_version = "pl: Version 5.0 (TDF %lu.%lu, Release %s)\n";
static char *pl_usage = "pl [-v] [-Ipath] [-g] [-V] infile.pl outfile.j";

int
main(int argc, char **argv)
{
    int i;
    int ok = 1;
    int diag = 0;
    char *in = NULL;
    char *out = NULL;
    init_includes ();
    for (i = 1 ; i < argc; i++) {
		char *a = argv [i];
		if (a [0] == '-') {
			switch (a [1]) {
			case 'g' : {
				if (a [2]) ok = 0;
				diag = 1;
				break;
			}
			case 'I' : {
				add_include (a + 2);
				break;
			}
			case 'v' : {
				if (a [2]) ok = 0;
				do_pp = 1;
				break;
			}
			case 'V' : {
				char *vc = RELEASE;
				unsigned long va = MAJOR_NO;
				unsigned long vb = MINOR_NO;
				if (a [2]) ok = 0;
				IGNORE fprintf (stderr, pl_version, va, vb, vc);
				break;
			}
			default : {
				ok = 0;
				break;
			}
			}
		} else {
			if (in == NULL) {
				in = a;
			} else if (out == NULL) {
				out = a;
			} else {
				ok = 0;
			}
		}
    }
    if (in == NULL || out == NULL) ok = 0;
    if (!ok) {
		IGNORE fprintf (stderr, "Error: Usage = %s\n", pl_usage);
		exit (EXIT_FAILURE);
    }
    in_file = fopen (in, "r");
    file_name = in;
    if (in_file == NULL) {
		IGNORE fprintf (stderr, "Error: Can't open input file, %s.\n", in);
		exit (EXIT_FAILURE);
    }
    out_file = fopen (out, "wb");
    if (out_file == NULL) {
		IGNORE fprintf (stderr, "Error: Can't open output file, %s.\n", out);
		exit (EXIT_FAILURE);
    }
    lex_v = reader ();
    init_units ();
    if (diag) line_no_tok = next_capsule_name (tok_ent);
    read_program ();
    IGNORE fclose (in_file);
    IGNORE fclose (out_file);
    return (0);
}
