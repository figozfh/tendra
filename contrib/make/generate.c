/*	$OpenPackages: generate.c,v 1.6 2001/05/29 15:29:24 espie Exp $ */
/*	$OpenBSD: generate.c,v 1.4 2001/05/23 12:34:43 espie Exp $ */
/* $TenDRA$ */

/*
 * Copyright (c) 2001 Marc Espie.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE OPENBSD PROJECT AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OPENBSD
 * PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "stats.h"
#include "ohash.h"
#include "cond_int.h"
#include "var_int.h"

#define M(x)	x, #x
char *table_var[] = {
	M(TARGET),
	M(OODATE),
	M(ALLSRC),
	M(IMPSRC),
	M(PREFIX),
	M(ARCHIVE),
	M(MEMBER),
	M(LONGTARGET),
	M(LONGOODATE),
	M(LONGALLSRC),
	M(LONGIMPSRC),
	M(LONGPREFIX),
	M(LONGARCHIVE),
	M(LONGMEMBER),
	M(FTARGET),
	M(DTARGET),
	M(FPREFIX),
	M(DPREFIX),
	M(FARCHIVE),
	M(DARCHIVE),
	M(FMEMBER),
	M(DMEMBER),
	NULL
};

char *table_cond[] = {
	M(COND_IF),
	M(COND_IFDEF),
	M(COND_IFNDEF),
	M(COND_IFMAKE),
	M(COND_IFNMAKE),
	M(COND_ELSE),
	M(COND_ELIFDEF),
	M(COND_ELIFNDEF),
	M(COND_ELIFMAKE),
	M(COND_ELIFNMAKE),
	M(COND_ENDIF),
	M(COND_FOR),
	M(COND_INCLUDE),
	M(COND_UNDEF),
	NULL
};


char **table[] = {
	table_var,
	table_cond
};

int
main(int argc, char *argv[])
{
	hv_type i;
	hv_type v;
	hv_type h;
	hv_type maxslots;
	hv_type slots;
	const char *e;
	char **occupied;
	char **t;
	int tn;

	Init_Stats();
	if (argc < 2)
		exit(1);

	tn = atoi(argv[1]);
	if (!tn)
		exit(1);
	t = table[tn-1];
	maxslots = 0;
	if (argc >= 3)
		maxslots = atoi(argv[2]);
	if (!maxslots)
		maxslots = 256;
	occupied = malloc(sizeof(char *) * maxslots);
	if (!occupied)
		exit(1);

	/* Find the correct value of slots: at least that many */
	slots = 0;
	for (i = 0; t[i] != NULL; i+=2)
		slots++;

	while (slots < maxslots) {
		for (i = 0; i < slots; i++)
			occupied[i] = NULL;
		for (i = 0; t[i] != NULL; i++) {
			e = NULL;
			v = ohash_interval(t[i], &e);
			h = v % slots;
			if (occupied[h])
				break;
			occupied[h] = t[i];
			i++;
		}
		if (t[i] == NULL) {
			printf("/* File created by generate %d %d, do not edit */\n", 
			    tn, slots);
			for (i = 0; t[i] != NULL; i++) {
				e = NULL;
				v = ohash_interval(t[i], &e);
				i++;
				printf("#define K_%s %u\n", t[i], v);
			}
			printf("#define MAGICSLOTS%d %u\n", tn, slots);
			exit(0);
		} else
			slots++;
	}
	exit(1);
}