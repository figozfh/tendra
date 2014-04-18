/* $Id$ */

/*
 * Copyright 2011, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 * Copyright 1993, Open Software Foundation, Inc.
 *
 * See doc/copyright/ for the full copyright terms.
 */

#include <shared/error.h>

#include "translat.h"
#include "labels.h"


static int last_label = 50;	/* >32 to avoid possible confusion with regs */


void seed_label(void)
{
  /*
   * Round label numbering up to next 100. Normally called at function start.
   * Purpose is to make assembler output more human-readable, and to make
   * minor code changes less likely to affect next function, so diff output
   * more compact.
   */
  int old_last_label = last_label;

  last_label = ((last_label + 100) / 100) * 100;

  (void) old_last_label;
  ASSERT(last_label >= old_last_label);
}


int new_label(void)
{
  last_label++;
  return last_label;
}


void set_label(int l)
{
  fprintf(as_file, "L.%d:\n", l);
}