/*
 * Automatically generated by lexi version 2.0
 * Copyright terms for the input source also apply to this generated code.
 */

#include <assert.h>
#include <string.h>

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ - 0L) >= 199901L
#include <stdbool.h>
#include <stdint.h>
#endif



	#include <shared/error.h>
	#include <shared/string.h>
	#include <shared/xalloc.h>

	#include "lctlexer.h"
	#include "lctsyntax.h"

	typedef int ZTTERMINAL;

	int curr_lct_token;
	int saved_lct_token;

	char lct_tokbuf[2000];
	static char *lct_token_end = lct_tokbuf + sizeof lct_tokbuf;
	static char *lct_token_current;

	char *lct_token_string;

	struct lct_state lct_state;

	static int
	lexi_getchar(struct lct_state *state)
	{
		int c;

		c = fgetc(state->input);

		if (c == EOF) {
			return LEXI_EOF;
		}

		if (c == '\n') {
			crt_line_no++;
		}

		return c;
	}

int lct_readchar(struct lct_state *state) {
	if (state->buffer_index) {
		return lct_pop(state);
	}

	return lexi_getchar(state);
}
void lct_push(struct lct_state *state, const int c) {
	assert(state);
	assert((size_t) state->buffer_index < sizeof state->buffer / sizeof *state->buffer);
	state->buffer[state->buffer_index++] = c;
}

int lct_pop(struct lct_state *state) {
	assert(state);
	assert(state->buffer_index > 0);
	return state->buffer[--state->buffer_index];
}

void lct_flush(struct lct_state *state) {
	state->buffer_index = 0;
}


/* LOOKUP TABLE */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ - 0L) >= 199901L
typedef uint8_t lookup_type;
#else
typedef unsigned char lookup_type;
#endif
static lookup_type lookup_tab[] = {
	   0,    0,    0,    0,    0,    0,    0,    0,    0,  0x1,  0x1,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,  0x1,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	 0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,    0,    0, 
	   0,    0,    0,    0,    0,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6, 
	 0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6, 
	 0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,    0,    0,    0,    0,  0x6, 
	   0,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6, 
	 0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6, 
	 0x6,  0x6,  0x6,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0
};

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ - 0L) >= 199901L
bool lct_group(enum lct_groups group, int c) {
#else
int lct_group(enum lct_groups group, int c) {
#endif
	if (c == LEXI_EOF) {
		return 0;
	}
	return lookup_tab[c] & group;
}


int lct_keyword(const char *identifier, int notfound) {
	if (streq(identifier, "ACTION")) return lct_kw_Haction;
	if (streq(identifier, "ARGUMENT")) return lct_kw_Hargument;
	if (streq(identifier, "HEADERS")) return lct_kw_Hheader;
	if (streq(identifier, "TRAILERS")) return lct_kw_Htrailer;
	return notfound;
}
/* PRE-PASS ANALYSERS */

void lct_init(struct lct_state *state, FILE_P_lct input) {
	state->zone = lct_next;
	state->buffer_index = 0;
	state->input = input;
}
/* ZONES PASS ANALYSER PROTOTYPES */

static int lct_next_code_ref(struct lct_state *state);
static int lct_next_code_ident(struct lct_state *state);
static int lct_next_code(struct lct_state *state);
static void lct_next_linecomment(struct lct_state *state);
static void lct_next_comment(struct lct_state *state);
static int lct_next_ident(struct lct_state *state);
/* MAIN PASS ANALYSERS */


/* MAIN PASS ANALYSER for code_ref */
static int
lct_next_code_ref(struct lct_state *state)
{
	start: {
		int c0 = lct_readchar(state);
		if (!lct_group(lct_group_alnum, c0)) {
			lct_push(state, c0);
			/* ACTION <fini_tokbuf> */
			{

	if (lct_token_current == lct_token_end) {
		error(ERR_FATAL, "Buffer overflow: trailing 0");
		*(lct_token_end - 1) = 0;
	} else {
		*lct_token_current++ = 0;
	}
			}
			/* END ACTION <fini_tokbuf> */
			return lct_code_Href;
		}

		/* DEFAULT */
		/* ACTION <push_tokbuf> */
		{

	if (lct_token_current == lct_token_end - 1) {
		error(ERR_FATAL, "Buffer overflow");
	} else {
		*lct_token_current++ = c0;
	}
		}
		/* END ACTION <push_tokbuf> */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for code_ident */
static int
lct_next_code_ident(struct lct_state *state)
{
	start: {
		int c0 = lct_readchar(state);
		if (!lct_group(lct_group_alnum, c0)) {
			lct_push(state, c0);
			/* ACTION <fini_tokbuf> */
			{

	if (lct_token_current == lct_token_end) {
		error(ERR_FATAL, "Buffer overflow: trailing 0");
		*(lct_token_end - 1) = 0;
	} else {
		*lct_token_current++ = 0;
	}
			}
			/* END ACTION <fini_tokbuf> */
			return lct_code_Hident;
		}

		/* DEFAULT */
		/* ACTION <push_tokbuf> */
		{

	if (lct_token_current == lct_token_end - 1) {
		error(ERR_FATAL, "Buffer overflow");
	} else {
		*lct_token_current++ = c0;
	}
		}
		/* END ACTION <push_tokbuf> */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for code */
static int
lct_next_code(struct lct_state *state)
{
	start: {
		int c0 = lct_readchar(state);
		switch (c0) {
		case LEXI_EOF: {
				return lct_code_Heof;
			}

		case '@': {
				int c1 = lct_readchar(state);
				switch (c1) {
				case '@': {
						return lct_code_Hat;
					}

				case '&': {
						int c2 = lct_readchar(state);
						if (lct_group(lct_group_alpha, c2)) {
							/* ACTION <init_tokbuf> */
							{

	lct_token_current = lct_tokbuf;
							}
							/* END ACTION <init_tokbuf> */
							/* ACTION <push_tokbuf> */
							{

	if (lct_token_current == lct_token_end - 1) {
		error(ERR_FATAL, "Buffer overflow");
	} else {
		*lct_token_current++ = c2;
	}
							}
							/* END ACTION <push_tokbuf> */
							return lct_next_code_ref(state);
						}
						lct_push(state, c2);
					}
					break;

				case '}': {
						state->zone = lct_next;
						return lct_code_Hend;
					}

				}
				if (lct_group(lct_group_alpha, c1)) {
					/* ACTION <init_tokbuf> */
					{

	lct_token_current = lct_tokbuf;
					}
					/* END ACTION <init_tokbuf> */
					/* ACTION <push_tokbuf> */
					{

	if (lct_token_current == lct_token_end - 1) {
		error(ERR_FATAL, "Buffer overflow");
	} else {
		*lct_token_current++ = c1;
	}
					}
					/* END ACTION <push_tokbuf> */
					return lct_next_code_ident(state);
				}
				lct_push(state, c1);
				return lct_lone_Hcode_Hat;
			}

		}

		/* DEFAULT */
		/* ACTION <code_string> */
		{
			ZTTERMINAL ZT1;

	size_t z, i;
	char *p;
	int c;

	z = 1024; /* arbitary non-zero initial size */
	p = NULL;
	i = 0;

	c = c0;

	do {
		if (p == NULL || i == z) {
			z *= 2;
			p = xrealloc(p, z);
		}

		p[i++] = c;

		c = lct_readchar(&lct_state);
	} while (c != '@' && c != LEXI_EOF);

	lct_token_string = xrealloc(p, i + 1);
	lct_token_string[i] = '\0';

	ZT1 = lct_code_Hstring;

	lct_push(&lct_state, c);
			return ZT1;
		}
		/* END ACTION <code_string> */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for linecomment */
static void
lct_next_linecomment(struct lct_state *state)
{
	start: {
		int c0 = lct_readchar(state);
		if (c0 == '\n') {
			return;
		}

		/* DEFAULT */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for comment */
static void
lct_next_comment(struct lct_state *state)
{
	start: {
		int c0 = lct_readchar(state);
		if (c0 == '*') {
			int c1 = lct_readchar(state);
			if (c1 == '/') {
				return;
			}
			lct_push(state, c1);
		}

		/* DEFAULT */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for ident */
static int
lct_next_ident(struct lct_state *state)
{
	start: {
		int c0 = lct_readchar(state);
		if (!lct_group(lct_group_alnum, c0)) {
			lct_push(state, c0);
			/* ACTION <fini_tokbuf> */
			{

	if (lct_token_current == lct_token_end) {
		error(ERR_FATAL, "Buffer overflow: trailing 0");
		*(lct_token_end - 1) = 0;
	} else {
		*lct_token_current++ = 0;
	}
			}
			/* END ACTION <fini_tokbuf> */
			/* ACTION <keyword> */
			{
				ZTTERMINAL ZT1;

	ZT1 = lct_keyword(lct_tokbuf, lct_ident);
				return ZT1;
			}
			/* END ACTION <keyword> */
		}

		/* DEFAULT */
		/* ACTION <push_tokbuf> */
		{

	if (lct_token_current == lct_token_end - 1) {
		error(ERR_FATAL, "Buffer overflow");
	} else {
		*lct_token_current++ = c0;
	}
		}
		/* END ACTION <push_tokbuf> */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for global zone */
int
lct_next(struct lct_state *state)
{
	if (state->zone != lct_next)
		return state->zone(state);
	start: {
		int c0 = lct_readchar(state);
		if (lct_group(lct_group_white, c0)) goto start;
		switch (c0) {
		case '@': {
				int c1 = lct_readchar(state);
				if (c1 == '{') {
					state->zone = lct_next_code;
					return lct_code_Hstart;
				}
				lct_push(state, c1);
			}
			break;

		case '/': {
				int c1 = lct_readchar(state);
				switch (c1) {
				case '/': {
						lct_next_linecomment(state);
						goto start;	/* pure function */
					}

				case '*': {
						lct_next_comment(state);
						goto start;	/* pure function */
					}

				}
				lct_push(state, c1);
			}
			break;

		case LEXI_EOF: {
				return lct_eof;
			}

		case '-': {
				int c1 = lct_readchar(state);
				if (c1 == '>') {
					return lct_arrow;
				}
				lct_push(state, c1);
			}
			break;

		case ',': {
				return lct_comma;
			}

		case ':': {
				return lct_colon;
			}

		case ';': {
				return lct_semicolon;
			}

		case '&': {
				return lct_ref;
			}

		case '=': {
				return lct_define;
			}

		case ')': {
				return lct_close;
			}

		case '(': {
				return lct_open;
			}

		}
		if (lct_group(lct_group_alpha, c0)) {
			/* ACTION <init_tokbuf> */
			{

	lct_token_current = lct_tokbuf;
			}
			/* END ACTION <init_tokbuf> */
			/* ACTION <push_tokbuf> */
			{

	if (lct_token_current == lct_token_end - 1) {
		error(ERR_FATAL, "Buffer overflow");
	} else {
		*lct_token_current++ = c0;
	}
			}
			/* END ACTION <push_tokbuf> */
			return lct_next_ident(state);
		}

		/* DEFAULT */
		return lct_unknown;
	}
}


