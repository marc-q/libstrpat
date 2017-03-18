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
 * @desc - Pattern to compile.
 *
 * Compiles the pattern described by @desc for
 * use with string_pat_match().
 *
 * Classes:
 * [:alnum:] - checks for an alphanumeric character.
 * [:alpha:] - checks  for  an  alphabetic  character.
 * [:blank:] - checks for a blank character; that is, a space or a tab.
 * [:cntrl:] - checks for a control character.
 * [:digit:] - checks for a digit (0 through 9).
 * [:graph:] - checks for any printable character except space.
 * [:lower:] - checks for a lowercase character.
 * [:print:] - checks for any printable character including space.
 * [:punct:] - Same as [:print:] except space or alphanumeric characters.
 * [:space:] - checks  for  white-space  characters.
 * [:upper:] -  checks for an uppercase letter.
 * [:xdigit:] - checks for hexadecimal digits (0 through F).
 *
 * Classes will be escaped with a backslash(\).
 *
 * The returned string should be freed with free().
 *
 * Return: Allocated string containing the compiled pattern.
 */
char*
string_pat_compile (const char *desc)
{
	const size_t desc_len = strlen (desc);
	char buf[250];
	
	// Empty string
	buf[0] = '\0';
	
	enum _pat_class class = PAT_CLASS_NONE;
	
	size_t j = 0;
	for (size_t i = 0; i < desc_len; i++, j++)
	{
		// Convert the class name to symbol
		if (string_match_start (&desc[i], "[:alnum:]"))
		{
			class = PAT_CLASS_ALNUM;
		}
		else if (string_match_start (&desc[i], "[:alpha:]"))
		{
			class = PAT_CLASS_ALPHA;
		}
		else if (string_match_start (&desc[i], "[:blank:]"))
		{
			class = PAT_CLASS_BLANK;
		}
		else if (string_match_start (&desc[i], "[:cntrl:]"))
		{
			class = PAT_CLASS_CNTRL;
		}
		else if (string_match_start (&desc[i], "[:digit:]"))
		{
			class = PAT_CLASS_DIGIT;
		}
		else if (string_match_start (&desc[i], "[:graph:]"))
		{
			class = PAT_CLASS_GRAPH;
		}
		else if (string_match_start (&desc[i], "[:lower:]"))
		{
			class = PAT_CLASS_LOWER;
		}
		else if (string_match_start (&desc[i], "[:print:]"))
		{
			class = PAT_CLASS_PRINT;
		}
		else if (string_match_start (&desc[i], "[:punct:]"))
		{
			class = PAT_CLASS_PUNCT;
		}
		else if (string_match_start (&desc[i], "[:space:]"))
		{
			class = PAT_CLASS_SPACE;
		}
		else if (string_match_start (&desc[i], "[:upper:]"))
		{
			class = PAT_CLASS_UPPER;
		}
		else if (string_match_start (&desc[i], "[:xdigit:]"))
		{
			class = PAT_CLASS_XDIGIT;
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
	return strdup (buf);
}

static bool
string_pat_match_char (const char c, const char **pat_cur, bool *escaped)
{
	// Check for escape sequence
	if (!(*escaped) &&
	    **pat_cur == PAT_ESCAPE)
	{
		*pat_cur += 1;
		*escaped = true;
	}
	
	// Escape sequence
	if (*escaped)
	{
		// Compare against the next character
		if (c == pat_cur[0][1])
		{
			*pat_cur += 2;
			*escaped = false;
			return true;
		}
		
		// Compare against the current class
		switch (**pat_cur)
		{
			case PAT_CLASS_ALNUM:
				return isalnum (c) != 0;
			case PAT_CLASS_ALPHA:
				return isalpha (c) != 0;
			case PAT_CLASS_BLANK:
				return isblank (c) != 0;
			case PAT_CLASS_CNTRL:
				return iscntrl (c) != 0;
			case PAT_CLASS_DIGIT:
				return isdigit (c) != 0;
			case PAT_CLASS_GRAPH:
				return isgraph (c) != 0;
			case PAT_CLASS_LOWER:
				return islower (c) != 0;
			case PAT_CLASS_PRINT:
				return isprint (c) != 0;
			case PAT_CLASS_PUNCT:
				return ispunct (c) != 0;
			case PAT_CLASS_SPACE:
				return isspace (c) != 0;
			case PAT_CLASS_UPPER:
				return isupper (c) != 0;
			case PAT_CLASS_XDIGIT:
				return isxdigit (c) != 0;
			default:
				break;
		}
	}
	
	// Compare against the current character
	if (c == **pat_cur)
	{
		*pat_cur += 1;
		*escaped = false;
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
	bool escaped = false;
	
	while (*str_cur && *pat_cur)
	{
		if (!string_pat_match_char (*str_cur, &pat_cur, &escaped))
		{
			return false;
		}
		str_cur++;
	}
	// Check if the whole pattern has been parsed
	return (*str_cur == '\0' && *pat_cur == '\0');
}
