/* Copyright 2017 Marc Volker Dickmann */
#include <stdio.h>
#include "libstrpat.h"

static void
test_pattern (const char *str, const char *desc)
{
	unsigned char *pat = NULL;
	pat = string_pat_compile (pat, desc);
	
	if (pat == NULL)
	{
		printf ("Error!!\n");
		return;
	}
	
	if (string_pat_match (str, pat))
	{
		printf ("Does Match!\n");
	}
	else
	{
		printf ("Does'nt Match!\n");
	}
	free (pat);
}

int
main (int argc, char *argv[])
{
	printf ("LibStrPat (C) 2017 Marc Volker Dickmann\n\n");
	
	test_pattern ("Test eins", "[:alpha:] eins");
	test_pattern ("Test eins", "[:alpha:] dins");
	
	test_pattern ("1. Punkt eins.", "[:digit:]. [:alnum:]");
	test_pattern (" 2. Punkt zwei.", "[:digit:]. [:alnum:]");
	
	test_pattern ("user@localhost.local", "[:alnum:]@[:alnum:].[:alpha:]");
	
	return 0;
}
