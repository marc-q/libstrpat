/* Copyright 2017 Marc Volker Dickmann */
#include <stdio.h>
#include "lib/dutils.h"
#include "libstrpat.h"

#define TESTS_AMOUNT 3
#define TEST_FAIL 0
#define TEST_PASS 1

static void
test_print_pass (const char *tstname)
{
	printf ("%s:", tstname);
	
	if (strlen (tstname) < 15)
	{
		printf ("\t\t\t");
	}
	else if (strlen (tstname) < 23)
	{
		printf ("\t\t");
	}
	else
	{
		printf ("\t");
	}
	
	printf ("%sPASS%s\n", BC_BLD_GREEN, BC_TXT_RST);
}

static void
test_print_fail (const char *tstname)
{
	printf ("%s:", tstname);
	
	if (strlen (tstname) < 15)
	{
		printf ("\t\t\t");
	}
	else if (strlen (tstname) < 23)
	{
		printf ("\t\t");
	}
	else
	{
		printf ("\t");
	}
	
	printf ("%sFAIL%s\n", BC_BLD_RED, BC_TXT_RST);
}

static void
test_print_summary (const int points)
{
	printf ("\n+=======================+\n");
	printf ("|  Summary of all tests |\n");
	printf ("+=======================+\n");
	printf ("|  Tests: \t%i\t|\n", TESTS_AMOUNT);
	printf ("+-----------------------+\n");
	printf ("|  Passed:\t%i\t|\n", points);
	printf ("|  Failed:\t%i\t|\n", TESTS_AMOUNT - points);
	printf ("+-----------------------+\n");
}

static int
test_class_none (void)
{
	int passed = TEST_FAIL;
	char *pat = string_pat_compile ("A simple, easy test.");
	
	if (string_pat_match ("A simple, easy test.", pat) &&
	    !string_pat_match ("A simple, asy test.", pat))
	{
		passed = TEST_PASS;
		test_print_pass ("CLASS_NONE");
	}
	else
	{
		test_print_fail ("CLASS_NONE");
	}
	
	free (pat);
	return passed;
}

static int
test_class_alnum (void)
{
	int passed = TEST_FAIL;
	char *pat = string_pat_compile ("[:alnum:].");
	
	if (string_pat_match ("AbC123DeF.", pat) &&
	    !string_pat_match ("AbC1 23DeF.", pat) &&
	    !string_pat_match ("AbC1_23DeF.", pat) &&
	    !string_pat_match ("AbC123DeF", pat))
	{
		passed = TEST_PASS;
		test_print_pass ("CLASS_ALNUM");
	}
	else
	{
		test_print_fail ("CLASS_ALNUM");
	}
	
	free (pat);
	return passed;
}

static int
test_class_alpha (void)
{
	int passed = TEST_FAIL;
	char *pat = string_pat_compile ("[:alpha:].");
	
	if (string_pat_match ("AbCDeF.", pat) &&
	    !string_pat_match ("AbC123DeF.", pat) &&
	    !string_pat_match ("AbC1_23DeF.", pat) &&
	    !string_pat_match ("AbC.DeF", pat) &&
	    !string_pat_match ("AbCDeF", pat))
	{
		passed = TEST_PASS;
		test_print_pass ("CLASS_ALPHA");
	}
	else
	{
		test_print_fail ("CLASS_ALPHA");
	}
	
	free (pat);
	return passed;
}

int
main (int argc, char *argv[])
{
	printf ("LibStrPat (C) 2017 Marc Volker Dickmann\n\n");
	
	int score = 0;
	
	score += test_class_none ();
	score += test_class_alnum ();
	score += test_class_alpha ();
	
	test_print_summary (score);
	return 0;
}
