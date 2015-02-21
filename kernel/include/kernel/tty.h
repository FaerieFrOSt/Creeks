
#ifndef _TTY_H_
# define _TTY_H_

#include <stddef.h>

void	terminal_initialize(void);
void	terminal_putchar(char c);
void	terminal_write(const char* data, size_t size);
void	scrollup(size_t n);

#endif /* !_TTY_H_ */
