/* $Id$ */

/*
 * Copyright 2002-2012, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

#ifndef config_key
#define config_key 1

#define NEWDIAGS 1
#define NEWDWARF 1
#define SHARED_ABBREV 1
#define has_asm 1
#define as_comment_symbol '/'

#define replace_compound 1
#define no_bitfield_ops 1
#define dont_unpad_apply 0
#define diagnose_registers 1
#define DWARF 0
#define load_ptr_pars 1
#define use_long_double 1
#define has64bits 1
#define keep_PIC_vars 0
#define target_dbl_maxexp 4932

#define value_of_null 0

#define remove_unused_counters 0
#define remove_unused_index_counters 0
#define good_index_factor(f)	0
#define good_pointer_factor(f)	((f)!= 1 && (f)!= 2 && (f)!= 4 && (f)!= 8)

#define temp_mips 0

#endif
