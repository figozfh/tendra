/*
 * Automatically generated by lexi version 2.0
 * Copyright terms for the input source also apply to this generated code.
 */

#ifndef LEXI_GENERATED_HEADER_lxi__INCLUDED
#define LEXI_GENERATED_HEADER_lxi__INCLUDED

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ - 0L) >= 199901L
#include <stdbool.h>
#endif



	extern char tokbuf[];
	extern char *token_end;
	extern int curr_lex_token;
	extern int saved_lex_token;
	extern unsigned int numbuf;
	extern struct lxi_state lxi_state;

	typedef FILE *FILE_P;

	#define CURRENT_LXI_TERMINAL curr_lex_token
	#define ADVANCE_LXI_LEXER    curr_lex_token = lxi_next(&lxi_state)
	#define SAVE_LXI_LEXER(T)    (saved_lex_token = curr_lex_token, curr_lex_token = (T))
	#define RESTORE_LXI_LEXER    (curr_lex_token = saved_lex_token)

/*
 * This struct holds state for the lexer; its representation is
 * private, but present here for ease of allocation.
 */
struct lxi_state {
	int (*zone)(struct lxi_state *);

	/*
	 * Lexi's buffer is a simple stack.
	 */
	int buffer[4];
	int buffer_index;
	FILE_P input;
};

/* Read a character */
int lxi_readchar(struct lxi_state *state);

/* Push a character to lexi's buffer */
void lxi_push(struct lxi_state *state, const int c);

/* Pop a character from lexi's buffer */
int lxi_pop(struct lxi_state *state);

/* Flush lexi's buffer */
void lxi_flush(struct lxi_state *state);

#ifndef LEXI_EOF
#define LEXI_EOF -1
#endif

enum lxi_groups {
	lxi_group_alnumhy = 0x1,
	lxi_group_alnum = 0x2,
	lxi_group_digit = 0x4,
	lxi_group_alpha = 0x8,
	lxi_group_white = 0x10,
	lxi_sidident_white = 0,
	lxi_ident_white = 0,
	lxi_string_white = 0,
	lxi_linecomment_white = 0,
	lxi_comment_white = 0
};

/* true if the given character is present in the given group */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ - 0L) >= 199901L
bool lxi_group(enum lxi_groups group, int c);
#else
int lxi_group(enum lxi_groups group, int c);
#endif

/* Identify a keyword */
int lxi_keyword(const char *identifier, int notfound);

/* Identify a token */
int lxi_next(struct lxi_state *state);

/* Initialise a lxi_state structure */
void lxi_init(struct lxi_state *state, FILE_P input);


#endif

