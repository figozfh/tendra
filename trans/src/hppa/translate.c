/*
 * Copyright 2011-2012, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

/*
 *  Translation is controlled by translate() in this module.
 *
 *  Code generation follows the following phases:
 *
 *  1. The TDF is read in, applying bottom-up optimisations.
 *  2. Top-down optimisations are performed.
 *  3. Register allocation is performed, and TDF idents introduced
 *     so code generation can be performed with no register spills.
 *  4. Code is generated for each procedure, and global declarations processed.
 *  5. Currently assembler source is generated directly, and the
 *     assembler optimiser (as -O) used for delay slot filling,
 *     instruction scheduling and peep-hole optimisation.
 *
 *  In a little more detail:
 *
 *  1) During the TDF reading process for tag declarations and tag
 *  definitions, applications of tokens are expanded as they are
 *  encountered, using the token definitions.  Every token used must have
 *  been previously defined in the bitstream.
 *
 *  The reading of the tag definitions creates a data structure in memory
 *  which directly represents the TDF.  At present, all the tag definitions
 *  are read into memory in this way before any further translation is
 *  performed.  This will shortly be changed, so that translation is
 *  performed in smaller units.  The translator is set up already so that
 *  the change to translate in units of single global definitions (or
 *  groups of these) can easily be made.
 *
 *  During the creation of the data structure bottom-up optimisations
 *  are performed.  These are the optimisations which can be done when a
 *  complete sub-tree of TDF is present, and are independent of the context
 *  in which the sub-tree is used.  They are defined in refactor.c and
 *  refactor_id.c.  These optimisation do such things as use the commutative
 *  and associative laws for plus to collect together and evaluate
 *  constants.  More ambitious examples of these bottom-up optimisations
 *  include all constant evaluation, elimination of inaccessible code, and
 *  forward propagation of assignments of constants.
 *
 *  2) After reading in the TDF various optimisations are performed which
 *  cannot be done until the whole context is present.  For example,
 *  constants cannot be extracted from a loop when we just have the loop
 *  itself, because we cannot tell whether the variables used in it are
 *  aliased.
 *
 *  These optimisations are invoked by opt_all_exps which is defined in
 *  indep.c.  They include extraction of constants from loops, common
 *  expression elimination on them and strength reduction for indexing.
 *
 *  3) Allocatable registers are partitioned into two sets, the s regs
 *  which are preserved over calls, and the t regs which are not.
 *
 *  The TDF is scanned introducing TDF idents so that expressions can be
 *  evaluated within the available t regs with no spills.  These new idents
 *  may be later allocated to a s reg later, if the weighting algorithm
 *  (below) considers this worth while.  Otherwise they will be on the stack.
 *
 *  Information is collected to help in global register allocation.  During
 *  a forward recursive scan of the TDF the number of accesses to each
 *  variable is computed (making assumptions about the frequency of
 *  execution of loops).  Then on the return scan of this recursion, for
 *  each declaration, the number of registers which must be free for it to
 *  be worthwhile to use a register is computed, and put into the TDF as
 *  the"break" point.  The procedures to do this are defined in weights.c.
 *
 *  Suitable idents not alive over a procedure call are allocated to a t reg,
 *  and others to s regs.  At the same time stack space requirements are
 *  calculated, so this is known before code for a procedure is generated.
 *
 *  4) Finally the code is generated without register spills.  The code is
 *  generated by make_code() in make_code.c, and make_XXX_code() in proc.c.
 *
 *  Note that procedure inlining and loop unrolling optimisations are not
 *  currently implemented.  Library procedures such as memcpy() and
 *  strcpy() are not treated specially.  Integer multiply, divide and
 *  remainder use the standard support procedures .mul, .div, .rem and
 *  unsigned variants.
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <shared/bool.h>
#include <shared/check.h>
#include <shared/error.h>
#include <shared/string.h>
#include <shared/xalloc.h>

#include <local/ash.h>
#include <local/out.h>

#include <tdf/nat.h>
#include <tdf/shape.h>
#include <tdf/tag.h>

#include <reader/exp.h>
#include <reader/token.h>
#include <reader/basicread.h>
#include <reader/externs.h>

#include <construct/installtypes.h>
#include <construct/dec.h>
#include <construct/ash.h>
#include <construct/exp.h>
#include <construct/installglob.h>

#include <flpt/flpt.h>

#include <main/driver.h>
#include <main/flags.h>
#include <main/print.h>

#include <refactor/optimise.h>

#include <diag3/diag_reform.h>

#include "procrec.h"
#include "frames.h"
#include "tempdecs.h"
#include "weights.h"
#include "regalloc.h"
#include "code_here.h"
#include "make_code.h"
#include "eval.h"
#include "bits.h"
#include "scan.h"
#include "getregs.h"
#include "reg.h"
#include "labels.h"
#include "stabs_diag3.h"
#include "translate.h"
#include "inst_fmt.h"
#include "getregs.h"
#include "special.h"
#include "ops.h"
#include "time.h"
#include "labexp.h"

int maxfix_tregs; /* the number of t regs allocatable */

char *proc_name;
char export[128];
labexp current, first;

int nexps;

extern int res_label;

dec **main_globals;
static int main_globals_index;

procrec *procrecs, *cpr;

#define is_zero(e) is_comm(e)
#define TRANSLATE_GLOBALS_FIRST 1

void
insection(enum section s)
{
	static enum section current_section = no_section;

	if (s == current_section) {
		return;
	}

	current_section = s;

	switch (s) {
	case shortdata_section:
		outs("\t.SHORTDATA\n");
		return;

	case data_section:
		outs("\t.DATA\n");
		return;

	case text_section:
		outs("\t.CODE\n");
		return;

	case bss_section:
	case shortbss_section:
		if (assembler == ASM_GAS) {
			/* gnu as does not recognise .BSS directive */
			outs("\t.SPACE\t$PRIVATE$\n");
			outs("\t.SUBSPA\t$BSS$\n");
		} else {
			outs("\t.BSS\n");
		}
		return;

	case no_section:
		current_section = no_section;
		return;

	case rodata_section:
	default:
		;
	}

	error(ERR_INTERNAL, "bad \".section\" name");
}

static void
mark_unaliased(exp e)
{
	exp p;
	bool ca = 1;

	/* don't know about aliases in other units */
	assert(!separate_units);

	for (p = pt(e); p != NULL && ca; p = pt(p)) {
		if (next(p) == NULL) {
			ca = 0;
		} else {
			if (!(p->last && next(p)->tag == cont_tag) &&
			    !(!p->last && next(p)->last && next(next(p))->tag == ass_tag)) {
				ca = 0;
			}
		}
	}

	if (ca) {
		setcaonly(e);
	}
}

/* translate the TDF */
void
local_translate_capsule(void)
{
	dec *d, **proc_def_trans_order;
	int *src_line = 0, next_proc_def;
	space tempregs;
	int noprocs;
	int procno;
	int r;
	int i;

	static int capn = 0;
	capn++;

	/* init nowhere */
	setregalt(nowhere.answhere, 0);
	nowhere.ashwhere.ashsize = 0;
	nowhere.ashwhere.ashsize = 0;

	/* First label; avoid conflict with reg nos (and backward compatibility) */
	crt_labno = 101;

	if (diag != DIAG_NONE) {
		init_stab();
	}

	/* mark the as output as TDF compiled */
	outs("\t;  Produced by the DERA TDF->HP PA-RISC translator ");
	outnl();
	outnl();
	outnl();

	outs("\t.SPACE  $TEXT$,SORT=8\n");
	outs("\t.SUBSPA $CODE$,QUAD=0,ALIGN=8,ACCESS=44,CODE_ONLY,SORT=24\n");
	outnl();

	outs("\t.SPACE  $PRIVATE$,SORT=16\n");
	outs("\t.SUBSPA $DATA$,QUAD=1,ALIGN=8,ACCESS=31,SORT=16\n\n");
	outs("\t.IMPORT\t$$dyncall,CODE\n");

	if (do_profile) {
		outs("\t.IMPORT\t_mcount,CODE\n");
	}

	outs("\t.IMPORT\t$global$,DATA\n");
	outnl();

#if 0
	outs("LB\t.MACRO\tTARGET\n");
	outs("\tldil\tL'TARGET,%r1\n");
	outs("\tldo\tR'TARGET(%r1),%r1\n");
	outs("\tbv\t0(%r1)\n");
	outs("\tnop\n");
	outnl();
#endif

	/* Begin diagnostics if necessary. */
	if (diag != DIAG_NONE) {
		outs("\t.CODE\n");
		outnl();
		init_stab_aux();
		outnl();
		outnl();
	}

	setregalt(nowhere.answhere, 0);
	nowhere.ashwhere.ashsize = 0;
	nowhere.ashwhere.ashsize = 0;

	if (diag == DIAG_NONE) {
		opt_all_exps(); /* optimise */
	}

	/* mark static unaliased; count procs */
	noprocs = 0;
	for (d = top_def; d != NULL; d = d->next) {
		exp crt_exp = d->exp;
		exp scexp = child(crt_exp);

		if (scexp == NULL) {
			continue;
		}

		if (diag == DIAG_NONE && !separate_units &&
			!d->extnamed && isvar(crt_exp)) {
			mark_unaliased(crt_exp);
		}

		if (scexp->tag == proc_tag || scexp->tag == general_proc_tag) {
			noprocs++;

			if (dyn_init && !strncmp("__I.TDF", d->name, 7)) {
				char *s;
				static char dyn = 0;

				if (!dyn) {
					outs("\t.SPACE  $PRIVATE$,SORT=16\n");
					outs("\t.SUBSPA $DYNDATA$,QUAD=1,ALIGN=4,ACCESS=31,SORT=16\n");
					outnl();
					dyn = 1;
				}

				s = xcalloc(64, sizeof(char));
				sprintf(s, "_GLOBAL_$I%d", capn);
				strcat(s, d->name + 7);
				d->name = s;

				if (assembler == ASM_HP) {
					asm_printop(".WORD %s", s);
				}
			}
		}
	}

	outnl();

	/* alloc memory */
	if (noprocs == 0) {
		procrecs = NULL;

		proc_def_trans_order = NULL;
	} else {
		procrecs = xcalloc(noprocs, sizeof (procrec));

		proc_def_trans_order = xcalloc(noprocs, sizeof (dec *));
		if (diag == DIAG_XDB) {
			src_line = xcalloc(noprocs, sizeof (int));
		}
	}

	/* number proc defs */
	procno = 0;
	for (d = top_def; d != NULL; d = d->next) {
		exp crt_exp = d->exp;

		if (child(crt_exp) != NULL && (child(crt_exp)->tag == proc_tag ||
		                             child(crt_exp)->tag == general_proc_tag))
		{
			procrec *pr = &procrecs[procno];
			proc_def_trans_order[procno] = d;

			/*
			 * Retrieve diagnostic info neccessary to comply with xdb's
			 * requirement that procedures be compiled in source file order.
			 */
			if (diag == DIAG_XDB) {
				diag_descriptor *dd = d->diag_info;

				if (dd != NULL) {
					sourcemark *sm = &dd -> data.id.whence;
					src_line[procno] = sm->line_no.nat_val.small_nat;
				} else {
					src_line[procno] = 0;
				}
			}

			pr->nameproc = next(crt_exp);
			no(child(crt_exp)) = procno++;/* index into procrecs in no(proc) */
		}
	}

	/*
	 * Scan to put everything in HP_PA form, and calculate register and stack
	 * space needs.
	 */

	/*
	 * First work out which fixed point t-regs, i.e. those not preserved
	 * over calls, can be used. This needs to be done before scan() which
	 * adds idents so temp reg needs are within available temp reg set.
	 */

	/* initial reg sets */
	tempregs.fixed = PROC_TREGS;
	tempregs.flt = PROC_FLT_TREGS;

	/* GR0,GR1,SP,DP are NEVER allocatable */
	tempregs.fixed |= RMASK(GR0);
	tempregs.fixed |= RMASK(GR1);
	tempregs.fixed |= RMASK(SP);
	tempregs.fixed |= RMASK(DP);

	if (PIC_code) {
		tempregs.fixed |= RMASK(GR19); /* %r19 is reserved in PIC mode */
	}

	/* count t fixed point regs we can use, and set the global maxfix_tregs */
	maxfix_tregs = 0;
	for (r = R_FIRST; r <= R_LAST; r++) {
		/* bit clear means allocatable */
		if (IS_TREG(r) && (tempregs.fixed & RMASK(r)) == 0) {
			maxfix_tregs++;
		}
	}

	asm_comment("maxfix_tregs=%d(%#lx) maxfloat_tregs=%d(%#lx)",
	            maxfix_tregs, (unsigned long) tempregs.fixed, MAXFLOAT_TREGS, (unsigned long) tempregs.flt);

	/* scan all the procs, to put everything in HP_PA form */
	nexps = 0;
	for (d = top_def; d != NULL; d = d->next) {
		exp crt_exp = d->exp;

		if (child(crt_exp) != NULL && (child(crt_exp)->tag == proc_tag ||
		                             child(crt_exp)->tag == general_proc_tag)) {
			procrec *pr = &procrecs[no(child(crt_exp))];
			exp *st = &child(crt_exp);
			cpr = pr;
			cpr->Has_ll = 0;
			cpr->Has_checkalloc = 0;
			hppabuiltin = 0;
			pr->needsproc = scan(st, &st);
			pr->callee_sz = callee_sz;
			pr->needsproc.builtin = hppabuiltin;
		}
	}

	/* calculate the break points for register allocation */
	for (d = top_def; d != NULL; d = d->next) {
		exp crt_exp = d->exp;

		if (child(crt_exp) != NULL && (child(crt_exp)->tag == proc_tag ||
		                             child(crt_exp)->tag == general_proc_tag)) {
			procrec *pr = &procrecs[no(child(crt_exp))];
			needs * ndpr = & pr->needsproc;
			long pprops = (ndpr->propneeds);
			bool leaf = (pprops & anyproccall) == 0;
			spacereq forrest;
			int freefixed, freefloat;
			proc_name = d->name;

			setframe_flags(child(crt_exp), leaf);

			/* free s registers = GR3,GR4,..,GR18 */
			freefixed = 16;

			if (Has_fp) { /* Has frame pointer */
				freefixed--;
				/* reserve GR3 as frame pointer (i.e. points to bottom of stack) */
			}

			if (Has_vsp) { /* Has variable stack pointer */
				freefixed--;
				/* reserve GR4 for use as copy of the original stack pointer */
			}

			if (is_PIC_and_calls) {
				freefixed--;
				/* best reserve GR5 for use as a copy of GR19 */
			}

			if (Has_vcallees) {
				pr->callee_sz = 0; /*  Don't know callee_sz  */
			}

			real_reg[1] = GR4;
			real_reg[2] = GR5;
			if (Has_fp) {
				if (is_PIC_and_calls && !Has_vsp) {
					real_reg[2] = GR4;
				}
			} else {
				if (Has_vsp) {
					if (is_PIC_and_calls) {
						real_reg[2] = GR3;
					} else {
						real_reg[1] = GR3;
					}
				} else if (is_PIC_and_calls) {
					real_reg[1] = GR3;
					real_reg[2] = GR4;
				}
			}

			/* +++ create float s regs for leaf? */
			freefloat = 0;		/* none, always the same */

			/* reg and stack allocation for tags */
			forrest = regalloc(next(child(child(crt_exp))), freefixed, freefloat, 0);

			/* reg and stack allocation for tags */
			pr->spacereqproc = forrest;

			set_up_frame(child(crt_exp));
		}
	}

	/*  Set up main_globals and output global definitions. */
	i = 0;
	for (d = top_def; d != NULL; d = d->next) {
		i++;
	}

	main_globals_index = i;
	if (main_globals_index != 0) {
		main_globals = xcalloc(main_globals_index, sizeof(dec*));
	} else {
		main_globals = NULL;
	}

	i = 0;
	for (d = top_def; d != NULL; d = d->next) {
		main_globals[i] = d;
		main_globals[i]->sym_number = i;
		i++;
	}

	for (d = top_def; d != NULL; d = d->next) {
		exp tag       = d->exp;
		char *name    = d->name;
		bool extnamed = d->extnamed;

		if (child(tag) == NULL && no(tag) != 0 && extnamed) {
			outs("\t.IMPORT\t");
			outs(name);
			outs(sh(tag)->tag == prokhd ? (isvar(tag) ? ",DATA\n" : ",CODE\n") : ",DATA\n");
		} else if (child(tag) != NULL && (extnamed || no(tag) != 0)) {
			if (child(tag)->tag != proc_tag && child(tag)->tag != general_proc_tag) {
				/* evaluate all outer level constants */
				instore is;
				long symdef = d->sym_number + 1;

				if (isvar(tag)) {
					symdef = -symdef;
				}

				if (extnamed && !(is_zero(child(tag)))) {
					outs("\t.EXPORT\t");
					outs(name);
					outs(",DATA\n");
				}

				is = evaluated(child(tag), symdef);
				if (diag != DIAG_NONE) {
					diag3_driver->stab_global(d->diag_info, child(tag), name, extnamed);
				}

				if (is.adval) {
					setvar(tag);
				}
			}
		}
	}

	/* Uninitialized data local to module. */

	for (d = top_def; d != NULL; d = d->next) {
		exp tag       = d->exp;
		char *name    = d->name;
		bool extnamed = d->extnamed;

		if (child(tag) == NULL && no(tag) != 0 && !extnamed) {
			shape s = d->shape;
			ash a;
			long size;
			int align;

			a = ashof(s);
			size  = (a.ashsize + 7) >> 3;
			align = ((a.ashalign > 32 || a.ashsize > 32) ? 8 : 4);

			if (size > 8) {
				insection(bss_section);
			} else {
				insection(shortbss_section);
			}

			outs("\t.ALIGN\t");
			outn(align);
			outs(name);
			outs("\t.BLOCKZ\t");
			outn(size);
		}
	}

	/* Translate the procedures. */

	/*
	 * XDB requires the procedures to be translated in the order
	 * that they appear in the c source file.
	 */
	if (diag == DIAG_XDB) {
		int n, j;

		for (n = 0; n < noprocs; n++) {
			for (j = n + 1; j < noprocs; j++) {
				int srcl;
				dec *pdef;

				if (src_line[n] <= src_line[j]) {
					continue;
				}

				srcl = src_line[n];
				src_line[n] = src_line[j];
				src_line[j] = srcl;

				pdef = proc_def_trans_order[n];
				proc_def_trans_order[n] = proc_def_trans_order[j];
				proc_def_trans_order[j] = pdef;
			}
		}
	} else {
#if TRANSLATE_GLOBALS_FIRST
		/*  Translate the global procedures first.  */
		int fstat = 0, lglob = noprocs - 1;
		while (fstat < lglob) {
			while (fstat < noprocs && proc_def_trans_order[fstat] ->extnamed) {
				fstat++;
			}

			while (lglob > 0 && !proc_def_trans_order[lglob] ->extnamed) {
				lglob--;
			}

			if (fstat < lglob) {
				dec *pdef;
				pdef = proc_def_trans_order[fstat];
				proc_def_trans_order[fstat] = proc_def_trans_order[lglob];
				proc_def_trans_order[lglob] = pdef;

				fstat++;
				lglob--;
			}
		}
#endif
	}

	for (next_proc_def = 0; next_proc_def < procno; next_proc_def++) {
		exp tag;
		char *name;
		bool extnamed;

		d        = proc_def_trans_order[next_proc_def];
		tag      = d->exp;
		name     = d->name;
		extnamed = d->extnamed;

		if (no(tag) != 0 || extnamed) {
			insection(text_section);
			outnl();
			outnl();

			if (diag != DIAG_NONE) {
				diag3_driver->stab_proc(d->diag_info, child(tag), name, extnamed);
			}

			seed_label(); /* reset label sequence */
			settempregs(child(tag)); /* reset getreg sequence */

			first = xmalloc(sizeof (struct labexp_t));
			first->e    = NULL;
			first->next = NULL;
			current = first;

			proc_name = name;
			code_here(child(tag), tempregs, nowhere);

			outs("\t.PROCEND\n\t;");
			outs(name);

			if (diag == DIAG_XDB) {
#if _SYMTAB_INCLUDED
				close_function_scope(res_label);
				outnl();
				outs("_");
				outs(name);
				outs("_end_");
#endif
			}
			outnl();
			outnl();

			if (extnamed) {
				outs("\t.EXPORT ");
				outs(name);
				outs(",ENTRY");
				outs(export);
				outnl();
				outnl();
				outnl();
			}

			if (first->next != 0) {
				exp e, z;
				labexp p, next;
				ash a;
				int lab, byte_size;

				outs("\n\n");
				next = first->next;
				do {
					e = next->e;
					z = e;
					a = ashof(sh(e));
					lab = next->lab;

					if (is_zero(e)) {
						byte_size = (a.ashsize + 7) >> 3;
						if (byte_size > 8) {
							insection(bss_section);
						} else {
							insection(shortbss_section);
						}

						if (a.ashalign > 32 || a.ashsize > 32) {
							set_align(64);
						} else {
							set_align(32);
						}

						outs(ext_name(lab));
						outs("\t.BLOCK\t");
						outn(byte_size);
						outnl();
					} else {
						insection(data_section);
						if (a.ashalign > 32 || a.ashsize > 32) {
							set_align(64);
						} else {
							set_align(32);
						}

						outs(ext_name(lab));
						outnl();
						evalone(z, 0);

						if (a.ashalign > 32) {
							set_align(64);
						}
					}
					next = next->next;
				} while (next != 0);

				next = first;
				do {
					p = next->next;
					free(next);
					next = p;
				} while (next != 0);

				outs("\t.CODE\n\n\n");
			} else {
				free(first);
			}
		}
	}
}

void
exit_translator(void)
{
	outnl();
	outnl();
	outnl();
	outnl();

	import_millicode();
	if ((has & HAS_LONG_DOUBLE)) {
		import_long_double_lib();
	}

	outnl();
	outnl();

	if (diag == DIAG_XDB) {
#ifdef _SYMTAB_INCLUDED
		output_DEBUG();
		outnl();
		outnl();
#endif
	}

	outs("\t.END\n");
}

