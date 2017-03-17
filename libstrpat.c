/* Copyright 2017 Marc Volker Dickmann */
#include "libstrpat.h"

/**
 * string_match_start()
 * @str - String to search in.
 * @pat - Pattern to search for.
 *
 * Checks if the string @str starts with
 * the pattern @pat.
 *
 * Return: If the pattern is found true, otherwise false.
 */
bool
string_match_start (const char *str, const char *pat)
{
	const size_t pat_len = strlen (pat);
	
	return (strlen (str) >= pat_len &&
		strncmp (str, pat, pat_len) == 0);
}

/**
 * string_match_end()
 * @str - String to search in.
 * @pat - Pattern to search for.
 *
 * Checks if the string @str ends with
 * the pattern @pat.
 *
 * Return: If the pattern is found true, otherwise false.
 */
bool
string_match_end (const char *str, const char *pat)
{
	const size_t str_len = strlen (str);
	const size_t pat_len = strlen (pat);
	
	return (str_len > pat_len &&
		strncmp (&str[str_len - pat_len], pat, pat_len) == 0);
}

/**
 * string_pat_compile()
 * @pat - Will hold the compiled pattern.
 * @desc - Pattern to compile.
 *
 * Compiles the pattern described by @desc for
 * use with string_pat_match().
 *
 * Classes:
 * [:alnum:] - checks for an alphanumeric character.
 * [:alpha:] - checks  for  an  alphabetic  character.
 * [:digit:] - checks for a digit (0 through 9).
 * [:print:] - checks for any printable character including space.
 *
 * Classes are escaped with a backslash(\). 
 *
 * The returned string should be freed with free().
 *
 * Return: Allocated string containing the compiled pattern.
 */
char*
string_pat_compile (char *pat, const char *desc)
{
	const size_t desc_len = strlen (desc);
	char buf[250];
	
	// Empty string
	buf[0] = '\0';
	
	enum _pat_class class = PAT_CLASS_NONE;
	
	size_t j = 0;
	for (size_t i = 0; i < desc_len; i++, j++)
	{
		if (string_match_start (&desc[i], "[:alpha:]"))
		{
			class = PAT_CLASS_ALPHA;
		}
		else if (string_match_start (&desc[i], "[:digit:]"))
		{
			class = PAT_CLASS_DIGIT;
		}
		else if (string_match_start (&desc[i], "[:alnum:]"))
		{
			class = PAT_CLASS_ALNUM;
		}
		else if (string_match_start (&desc[i], "[:print:]"))
		{
			class = PAT_CLASS_PRINT;
		}
		else
		{
			// Escape the Escape character
			if (desc[i] == PAT_ESCAPE)
			{
				buf[j++] = PAT_ESCAPE; 
			}
			buf[j] = desc[i];
			continue;
		}
		
		// Escape the class symbol
		buf[j++] = PAT_ESCAPE;
		buf[j] = class;
		i += 8;
	}
	buf[j] = '\0';
	
	const size_t buf_len = strlen (buf) + 1;
	pat = malloc (sizeof (char) * buf_len);
	return memcpy (pat, buf, buf_len);
}

static bool
string_pat_match_char (const char c, const char **pat_cur, int *flags)
{
	// Check for escape sequence
	if (!((*flags >> PAT_FLAG_ESCAPED) & 0x1) &&
	    **pat_cur == PAT_ESCAPE)
	{
		*pat_cur += 1;
		*flags |= (1 << PAT_FLAG_ESCAPED);
	}
	
	// Escape sequence
	if (((*flags >> PAT_FLAG_ESCAPED) & 0x1))
	{
		// Compare against the next character
		if (c == pat_cur[0][1])
		{
			*pat_cur += 2;
			*flags &= ~(1 << PAT_FLAG_ESCAPED);
			return true;
		}
		
		// Compare against the current class
		switch (**pat_cur)
		{
			case PAT_CLASS_ALPHA:
				return isalpha (c) != 0;
			case PAT_CLASS_DIGIT:
				return isdigit (c) != 0;
			case PAT_CLASS_ALNUM:
				return isalnum (c) != 0;
			case PAT_CLASS_PRINT:
				return isprint (c) != 0;
			default:
				break;
		}
	}
	
	// Compare against the current character
	if (c == **pat_cur)
	{
		*pat_cur += 1;
		*flags &= ~(1 << PAT_FLAG_ESCAPED);
		return true;
	}
	return false;
}

/**
 * string_pat_match()
 * @str - String to search in.
 * @pat - Pattern to search for.
 *
 * Pattern recognition.
 *
 * Return: True if the pattern matches, otherwise false.
 */
bool
string_pat_match (const char *str, const char *pat)
{
	const char *str_cur = str;
	const char *pat_cur = pat;
	int flags = 0;
	
	while (*str_cur && *pat_cur)
	{
		if (!string_pat_match_char (*str_cur, &pat_cur, &flags))
		{
			return false;
		}
		str_cur++;
	}
	return true;
}
