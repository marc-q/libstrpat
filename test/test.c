/* Copyright 2017 Marc Volker Dickmann */
#include <stdio.h>
#include "../lib/dutils.h"
#include "../libstrpat.h"
#include "test.h"

static bool
test_match (const char *pattern, const char *str)
{
	char *pat = string_pat_compile (pattern);
	
	const bool r = string_pat_match (str, pat);
	
	free (pat);
	return r;
}

static void
test_matches (void)
{
	struct _match_test cases[] =
	{	
		// CLASS: NONE
		{true, NULL, "CLASS: NONE"},
		{true, "A simple, easy test.", "A simple, easy test."},
		{false, "A simple, easy test.",  "A simple, asy test."},
		
		// CLASS: ALNUM
		{true, NULL, "CLASS: ALNUM"},
		{true, "[:alnum:].", "AbC123DeF."},
		{false, "[:alnum:].", "AbC1 23DeF."},
		{false, "[:alnum:].", "AbC1_23DeF."},
		{false, "[:alnum:].", "AbC123DeF"},
		
		// CLASS: ALPHA
		{true, NULL, "CLASS: ALPHA"},
		{true, "[:alpha:].", "AbCDeF."},
		{false, "[:alpha:].", "AbC123DeF."},
		{false, "[:alpha:].", "AbC1_23DeF."},
		{false, "[:alpha:].", "AbC.DeF"},
		{false, "[:alpha:].", "AbCDeF"},
		
		// TEST: EMAIL
		{true, NULL, "TEST: E-MAIL"},
		{true, "[:graph:]@[:graph:].[:alpha:]", "u_s-er@l-o_calhost.local"},
		{false, "[:graph:]@[:graph:].[:alpha:]", "u_s-e r@l-o_calhost.local"},
		{false, "[:graph:]@[:graph:].[:alpha:]", "u_s-er@l-o_ca lhost.local"},
		
		// END
		{true, NULL, NULL}
	};
	
	struct _match_test *cur = cases;
	size_t case_id = 1;
	
	while (cur->str)
	{
		if (cur->pattern == NULL)
		{
			printf ("\n%s\n", cur->str);
			case_id = 1;
		}
		else if (test_match (cur->pattern, cur->str) == cur->match)
		{
			printf ("case: %zu\t%sPASSED%s\n", case_id++, BC_BLD_GREEN, BC_TXT_RST);
		}
		else
		{
			printf ("case: %zu\t%sFAILED%s\n", case_id++, BC_BLD_RED, BC_TXT_RST);
		}
		cur++;
	}
}

int
main (int argc, char *argv[])
{
	printf ("LibStrPat (C) 2017 Marc Volker Dickmann\n\n");
	
	test_matches ();
	
	return 0;
}
