/* 
 *  Copyright (c) 2002-2008 The TenDRA Project <http://www.tendra.org/>.
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 * 
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. Neither the name of The TenDRA Project nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific, prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 *  IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 *     		 Crown Copyright (c) 1997
 *     
 *     This TenDRA(r) Computer Program is subject to Copyright
 *     owned by the United Kingdom Secretary of State for Defence
 *     acting through the Defence Evaluation and Research Agency
 *     (DERA).  It is made available to Recipients with a
 *     royalty-free licence for its use, reproduction, transfer
 *     to other parties and amendment for any purpose not excluding
 *     product development provided that any such use et cetera
 *     shall be deemed to be acceptance of the following conditions:-
 *     
 *         (1) Its Recipients shall ensure that this Notice is
 *         reproduced upon any copies or amended versions of it;
 *     
 *         (2) Any amended version of it shall be clearly marked to
 *         show both the nature of and the organisation responsible
 *         for the relevant amendment or amendments;
 *     
 *         (3) Its onward transfer from a recipient to another
 *         party shall be deemed to be that party's acceptance of
 *         these conditions;
 *     
 *         (4) DERA gives no warranty or assurance as to its
 *         quality or suitability for any purpose and DERA accepts
 *         no liability whatsoever in relation to any use to which
 *         it may be put.
 * 
 * $Id$
 * 
 */


/*
 * Automatically generated by lexi version 2.0
 */

#include "lexer.h"


static char* token_current;
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/* LOOKUP TABLE */

typedef uint16_t lookup_type;
static lookup_type lookup_tab[257] = {
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0001, 0x0001, 0x0000,
	0x0000, 0x0001, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0001, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0010, 0x0000, 0x0000,
	0x001c, 0x001c, 0x001c, 0x001c,
	0x001c, 0x001c, 0x001c, 0x001c,
	0x001c, 0x001c, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x0000,
	0x0000, 0x0000, 0x0000, 0x001a,
	0x0000, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x001a,
	0x001a, 0x001a, 0x001a, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000
};

void lexi_push(struct lexi_state *state, const int c) {
	assert(state);
	assert(state->buffer_index < sizeof state->buffer / sizeof *state->buffer);
	state->buffer[state->buffer_index++] = c;
}

int lexi_pop(struct lexi_state *state) {
	assert(state);
	assert(state->buffer_index > 0);
	return state->buffer[--state->buffer_index];
}

void lexi_flush(struct lexi_state *state) {
	state->buffer_index = 0;
}

int lexi_readchar(struct lexi_state *state) {
	if(state->buffer_index) {
		return lexi_pop(state);
	}

	return lexi_getchar();
}

bool lexi_group(enum lexi_groups group, int c) {
	return lookup_tab[c] & group;
}


#include <string.h>
int lexi_keyword(const char *identifier, int notfound) {
	if(!strcmp(identifier, "ACTION")) return lex_action_Hkw;
	if(!strcmp(identifier, "COPYRIGHT")) return lex_copyright;
	if(!strcmp(identifier, "DEFAULT")) return lex_default;
	if(!strcmp(identifier, "ELSE")) return lex_else;
	if(!strcmp(identifier, "GROUP")) return lex_group;
	if(!strcmp(identifier, "IF")) return lex_if;
	if(!strcmp(identifier, "KEYWORD")) return lex_keyword;
	if(!strcmp(identifier, "MAPPING")) return lex_mapping;
	if(!strcmp(identifier, "TOKEN")) return lex_token;
	if(!strcmp(identifier, "TYPE")) return lex_type_Hkw;
	if(!strcmp(identifier, "ZONE")) return lex_zone;
	if(!strcmp(identifier, "white")) return lex_white;
	return notfound;
}
/* PRE-PASS ANALYSERS */

void lexi_init(struct lexi_state *state) {
	state->zone_function = lexi_read_token;
	state->buffer_index = 0;
}
/* ZONES PASS ANALYSER PROTOTYPES*/

static int lexi_read_token_sididentifierzone(struct lexi_state *state);
static int lexi_read_token_identifierzone(struct lexi_state *state);
static int lexi_read_token_stringzone(struct lexi_state *state);
static int lexi_read_token_line_comment(struct lexi_state *state);
static int lexi_read_token_comment(struct lexi_state *state);
static int lexi_read_token_arg_char_nb_zone(struct lexi_state *state);
/* MAIN PASS ANALYSERS */

/* MAIN PASS ANALYSER for zone sididentifierzone*/

static int
lexi_read_token_sididentifierzone(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		if (lexi_group(lexi_group_sididentifierzone_white, c0)) goto start;
		if (!lexi_group(lexi_group_alphanumhyphen, c0)) {
			lexi_push(state, c0);
			{

       	if(token_current==token_end) {
		error(ERROR_FATAL, "Buffer overflow: trailing 0");
	       *(token_end-1) = 0;		
	} else {
	       *token_current++ = 0;	
	}
			}
			return lex_sid_Hidentifier;
		}
		{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c0;
		}
		goto start;
	}
}
/* MAIN PASS ANALYSER for zone identifierzone*/

static int
lexi_read_token_identifierzone(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		if (lexi_group(lexi_group_identifierzone_white, c0)) goto start;
		if (!lexi_group(lexi_group_alphanum, c0)) {
			lexi_push(state, c0);
			{

       	if(token_current==token_end) {
		error(ERROR_FATAL, "Buffer overflow: trailing 0");
	       *(token_end-1) = 0;		
	} else {
	       *token_current++ = 0;	
	}
			}
			{
				int ZT1;

	ZT1 = lexi_keyword(token_buff, lex_identifier);
				return ZT1;
			}
		}
		{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c0;
		}
		goto start;
	}
}
/* MAIN PASS ANALYSER for zone stringzone*/

static int
lexi_read_token_stringzone(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		if (lexi_group(lexi_group_stringzone_white, c0)) goto start;
		if (c0 == '\n') {
			{

	error(ERROR_SERIOUS, "Unexpected newline in string");
			}
			goto start;
		} else if (c0 == '"') {
			{

       	if(token_current==token_end) {
		error(ERROR_FATAL, "Buffer overflow: trailing 0");
	       *(token_end-1) = 0;		
	} else {
	       *token_current++ = 0;	
	}
			}
			return lex_string;
		} else if (c0 == '\\') {
			int c1 = lexi_readchar(state);
			if (c1 == '"') {
				{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c1;
				}
				goto start;
			}
			lexi_push(state, c1);
		} else if (c0 == LEXI_EOF) {
			{

	error(ERROR_SERIOUS, "Unexpected eof in string");
			}
			goto start;
		}
		{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c0;
		}
		goto start;
	}
}
/* MAIN PASS ANALYSER for zone line_comment*/

static int
lexi_read_token_line_comment(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		if (lexi_group(lexi_group_line_comment_white, c0)) goto start;
		if (c0 == '\n') {
			return;
		}
		goto start;
	}
}
/* MAIN PASS ANALYSER for zone comment*/

static int
lexi_read_token_comment(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		if (lexi_group(lexi_group_comment_white, c0)) goto start;
		if (c0 == '*') {
			int c1 = lexi_readchar(state);
			if (c1 == '/') {
				return;
			}
			lexi_push(state, c1);
		}
		goto start;
	}
}
/* MAIN PASS ANALYSER for zone arg_char_nb_zone*/

static int
lexi_read_token_arg_char_nb_zone(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		if (lexi_group(lexi_group_white, c0)) goto start;
		if (!lexi_group(lexi_group_digit, c0)) {
			lexi_push(state, c0);
			return lex_arg_Hchar_Hnb;
		}
		{

	number_buffer *= 10;
	number_buffer += c0 - '0'; /*TODO do this in a safe way that does not assume ASCII or a coding where digits are contiguous*/
		}
		goto start;
	}
}
/* MAIN PASS ANALYSER for zone global*/

int
lexi_read_token(struct lexi_state *state)
{
	if(state->zone_function != lexi_read_token)
		return (*state->zone_function)(state);
	start: {
		int c0 = lexi_readchar(state);
		if (lexi_group(lexi_group_white, c0)) goto start;
		switch (c0) {
			case '!': {
				return lex_arg_Hignore;
			}
			case '"': {
				{

	token_current=token_buff;
				}
				return lexi_read_token_stringzone(state);
				goto start;
			}
			case '#': {
				int c1 = lexi_readchar(state);
				if (c1 == '#') {
					return lex_arg_Hchar_Hlist;
				} else if (c1 == '$') {
					return lex_arg_Hchar_Hvoid;
				} else if (c1 == '*') {
					return lex_arg_Hchar_Hstring;
				} else if (c1 == 'n') {
					return lex_arg_Hnb_Hof_Hchars;
				}
				if (lexi_group(lexi_group_digit, c1)) {
					{

	number_buffer = 0;
					}
					{

	number_buffer *= 10;
	number_buffer += c1 - '0'; /*TODO do this in a safe way that does not assume ASCII or a coding where digits are contiguous*/
					}
					return lexi_read_token_arg_char_nb_zone(state);
					goto start;
				}
				lexi_push(state, c1);
				break;
			}
			case '$': {
				int c1 = lexi_readchar(state);
				if (c1 == '$') {
					return lex_nothing_Hmarker;
				}
				if (lexi_group(lexi_group_alpha, c1)) {
					{

	token_current=token_buff;
					}
					{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c1;
					}
					return lexi_read_token_sididentifierzone(state);
					goto start;
				}
				lexi_push(state, c1);
				return lex_arg_Hreturn_Hterminal;
			}
			case '&': {
				return lex_reference;
			}
			case '(': {
				return lex_open;
			}
			case ')': {
				return lex_close;
			}
			case '+': {
				return lex_plus;
			}
			case ',': {
				return lex_comma;
			}
			case '-': {
				int c1 = lexi_readchar(state);
				if (c1 == '>') {
					return lex_arrow;
				}
				lexi_push(state, c1);
				break;
			}
			case '.': {
				int c1 = lexi_readchar(state);
				if (c1 == '.') {
					int c2 = lexi_readchar(state);
					if (c2 == '.') {
						return lex_range;
					}
					lexi_push(state, c2);
				}
				lexi_push(state, c1);
				break;
			}
			case '/': {
				int c1 = lexi_readchar(state);
				if (c1 == '*') {
					lexi_read_token_comment(state);
					goto start;
				} else if (c1 == '/') {
					lexi_read_token_line_comment(state);
					goto start;
				}
				lexi_push(state, c1);
				break;
			}
			case ':': {
				return lex_colon;
			}
			case ';': {
				return lex_semicolon;
			}
			case '<': {
				return lex_begin_Haction;
			}
			case '=': {
				return lex_equal;
			}
			case '>': {
				return lex_end_Haction;
			}
			case '[': {
				int c1 = lexi_readchar(state);
				if (c1 == '.') {
					int c2 = lexi_readchar(state);
					if (c2 == '.') {
						int c3 = lexi_readchar(state);
						if (c3 == '.') {
							int c4 = lexi_readchar(state);
							if (c4 == ')') {
								return lex_range_Hclosed_Hopen;
							} else if (c4 == ']') {
								return lex_range_Hclosed_Hclosed;
							}
							lexi_push(state, c4);
						}
						lexi_push(state, c3);
					}
					lexi_push(state, c2);
				}
				lexi_push(state, c1);
				break;
			}
			case '{': {
				int c1 = lexi_readchar(state);
				if (c1 == '0') {
					int c2 = lexi_readchar(state);
					if (c2 == '-') {
						int c3 = lexi_readchar(state);
						if (c3 == '9') {
							int c4 = lexi_readchar(state);
							if (c4 == '}') {
								return lex_digit;
							}
							lexi_push(state, c4);
						}
						lexi_push(state, c3);
					}
					lexi_push(state, c2);
				} else if (c1 == 'A') {
					int c2 = lexi_readchar(state);
					if (c2 == '-') {
						int c3 = lexi_readchar(state);
						if (c3 == 'Z') {
							int c4 = lexi_readchar(state);
							if (c4 == '}') {
								return lex_upper;
							}
							lexi_push(state, c4);
						}
						lexi_push(state, c3);
					}
					lexi_push(state, c2);
				} else if (c1 == 'a') {
					int c2 = lexi_readchar(state);
					if (c2 == '-') {
						int c3 = lexi_readchar(state);
						if (c3 == 'z') {
							int c4 = lexi_readchar(state);
							if (c4 == '}') {
								return lex_lower;
							}
							lexi_push(state, c4);
						}
						lexi_push(state, c3);
					}
					lexi_push(state, c2);
				}
				lexi_push(state, c1);
				return lex_open_Hbrace;
			}
			case '}': {
				return lex_close_Hbrace;
			}
			case LEXI_EOF: {
				return lex_eof;
			}
		}
		if (lexi_group(lexi_group_alpha, c0)) {
			{

	token_current=token_buff;
			}
			{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c0;
			}
			return lexi_read_token_identifierzone(state);
			goto start;
		}
		return lexi_unknown_token;
	}
}


