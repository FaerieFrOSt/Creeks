#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

static size_t	terminal_row;
static size_t	terminal_column;
static uint8_t	terminal_color;
static uint16_t	*terminal_buffer;

void	terminal_scrollup(size_t n)
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

void	terminal_removecolor(void)
{
	terminal_color = make_color(COLOR_GREEN, COLOR_BLACK);
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
			terminal_scrollup(1);
		}
	}
	else if (c == 9) // TAB
		terminal_column = terminal_column + 8 - (terminal_column % 8);
	else if (c == 13) //CR
		terminal_column = 0;
	else
	{
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH)
		{
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
			{
				--terminal_row;
				terminal_scrollup(1);
			}
		}
	}
	move_cursor(terminal_column, terminal_row);
}

void	terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; ++i)
		terminal_putchar(data[i]);
}

void	terminal_debug(const char *s)
{
	terminal_setcolor(make_color(COLOR_CYAN, COLOR_BLACK));
	terminal_write("KERNEL : ", 9);
	terminal_removecolor();
	terminal_write(s, strlen(s));
}

void	terminal_ok(void)
{
	terminal_setcolor(make_color(COLOR_WHITE, COLOR_BLACK));
	terminal_write("OK!\n", 4);
	terminal_removecolor();
}

void	terminal_warning(const char *s)
{
	terminal_setcolor(make_color(COLOR_MAGENTA, COLOR_BLACK));
	terminal_write("KERNEL : ", 9);
	terminal_removecolor();
	terminal_write(s, strlen(s));
}

void	terminal_error(const char *s)
{
	terminal_setcolor(make_color(COLOR_RED, COLOR_BLACK));
	terminal_write("KERNEL : ", 9);
	terminal_removecolor();
	terminal_write(s, strlen(s));
}

