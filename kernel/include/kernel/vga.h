
#ifndef _VGA_H_
# define _VGA_H_

#include <stdint.h>
#include "kernel/io.h"

#define VGA_WIDTH	80
#define VGA_HEIGHT	25
#define VGA_SIZE	VGA_WIDTH * VGA_HEIGHT
#define VGA_MEMORY	(uint16_t*)0xB8000

enum	vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GRAY = 7,
	COLOR_DARK_GRAY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

static inline uint8_t	make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t	make_vgaentry(char c, uint8_t color)
{
	uint16_t	c16 = c;
	uint16_t	color16 = color;
	return c16 | color16 << 8;
}

static inline void	move_cursor(uint8_t x, uint8_t y)
{
	uint16_t	pos;

	pos = y * VGA_WIDTH + x;
	outb(0x3d4, 0x0f);
	outb(0x3d5, (uint8_t) pos);
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint8_t) (pos >> 8));
}

#endif /* !_VGA_H_ */
