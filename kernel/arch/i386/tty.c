#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

size_t		terminal_row;
size_t		terminal_column;
uint8_t		terminal_color;
uint16_t	*terminal_buffer;

void	scrollup(size_t n)
{
	if (!n)
		return;
	uint16_t	*buff, *tmp;
	for (buff = VGA_MEMORY; buff < VGA_MEMORY + VGA_SIZE; ++buff)
	{
		tmp = buff + n * VGA_WIDTH;
		if (tmp < VGA_MEMORY + VGA_SIZE)
			*buff = *tmp;
		else
			*buff = make_vgaentry(' ', terminal_color);
	}
}

void	terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_GREEN, COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; ++y)
		for (size_t x = 0; x < VGA_WIDTH; ++x)
			terminal_buffer[y * VGA_WIDTH + x] = make_vgaentry(' ', terminal_color);
}

void	terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void	terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	terminal_buffer[y * VGA_WIDTH + x] = make_vgaentry(c, color);
}

void	terminal_putchar(char c)
{
	if (c == '\n')
	{
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
		{
			--terminal_row;
			scrollup(1);
		}
		return;
	}
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
		{
			--terminal_row;
			scrollup(1);
		}
	}
}

void	terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; ++i)
		terminal_putchar(data[i]);
}
