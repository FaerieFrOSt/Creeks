
#ifndef _TTY_H_
# define _TTY_H_

#include <stddef.h>
#include <stdint.h>

void	terminal_initialize(void);
void	terminal_putchar(char c);
void	terminal_write(const char* data, size_t size);
void	terminal_scrollup(size_t n);
void	terminal_setcolor(uint8_t color);
void	terminal_removecolor(void);

void	terminal_debug(const char*);
void	terminal_warning(const char*);
void	terminal_error(const char*);
void	terminal_ok(void);

#endif /* !_TTY_H_ */
