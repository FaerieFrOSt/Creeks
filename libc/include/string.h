
#ifndef _STRING_H_
# define _STRING_H_

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int		memcmp(const void*, const void*, size_t);
void	*memcpy(void* __restrict, const void* __restrict, size_t);
void	*memmove(void*, const void*, size_t);
void	*memset(void *str, int c, size_t n);
size_t	strlen(const char*);

#ifdef __cplusplus
}
#endif

#endif /* !_STRING_H_ */
