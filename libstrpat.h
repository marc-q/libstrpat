#ifndef __LIBSTRPAT_H__
#define __LIBSTRPAT_H__

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define PAT_ESCAPE '\\'

#define PAT_FLAG_ESCAPED 0

enum _pat_class
{
	PAT_CLASS_NONE = 0,
	PAT_CLASS_ALNUM = 'a',
	PAT_CLASS_ALPHA,
	
	PAT_CLASS_BLANK,
	
	PAT_CLASS_CNTRL,
	
	PAT_CLASS_DIGIT,
	
	PAT_CLASS_GRAPH,
	
	PAT_CLASS_LOWER,
	
	PAT_CLASS_PRINT,
	PAT_CLASS_PUNCT,
	
	PAT_CLASS_SPACE,
	
	PAT_CLASS_UPPER,
	
	PAT_CLASS_XDIGIT
};

bool string_match_start (const char*, const char*);
bool string_match_end (const char*, const char*);

char* string_pat_compile (const char*);
bool string_pat_match (const char*, const char*);

#endif /* __LIBSTRPAT_H__ */
