#include <string.h>

size_t	strlen(const char* string)
{
	size_t	res = 0;
	while (string[res])
		++res;
	return res;
}
