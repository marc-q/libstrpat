# libstrpat
[WIP] Will be a string pattern library.

## Example: E-Mail address
This program checks if the first argument is a dot com email address.
```C
#include <stdio.h>
#include "libstrpat.h"

int
main (int argc, char *argv[])
{
	if (argc == 2)
	{
		char *pat = string_pat_compile ("[:graph:]@[:graph:].com");
		
		if (string_pat_match (argv[1], pat))
		{
			printf ("This is a .com address.\n");
		}
		else
		{
			printf ("Not a .com address, sorry.\n");
		}
		
		free (pat);
	}
	else
	{
		printf ("Usage: %s <email_address>!\n", argv[0]);
	}

	return 0;
}
```
