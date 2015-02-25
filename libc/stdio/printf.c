#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void	print(const char *data, size_t data_length)
{
	for (size_t i = 0; i < data_length; ++i)
		putchar((int)((const unsigned char*)data)[i]);
}

static void	itoa(int nb, const char *base)
{
	size_t	size = strlen(base);
	if (!base || !size)
		return;
	if (nb < 0)
	{
		putchar((int)'-');
		nb = -nb;
	}
	if (nb / size)
		itoa(nb / size, base);
	putchar((int)base[nb % size]);
}

static void	dectoa(int nb)
{
	static const char	*base = "0123456789";
	itoa(nb, base);
}

static void	hextoa(int nb)
{
	static const char	*base = "0123456789ABCDEF";
	print("0x", 2);
	itoa(nb, base);
}

int	printf(const char *restrict format, ...)
{
	va_list	parameters;
	va_start(parameters, format);

	int	written = 0;
	size_t	amount;
	bool	rejected_bad_specifier = false;
	while (*format != '\0')
	{
		if (*format != '%')
		{
print_c:
			amount = 1;
			while (format[amount] && format[amount] != '%')
				++amount;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}
		const char	*format_begun_at = format;
		if (*(++format) == '%')
			goto print_c;
		if (rejected_bad_specifier)
		{
incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}

		if (*format == 'c')
		{
			++format;
			char	c = (char)va_arg(parameters, int);
			print(&c, sizeof(c));
		}else if (*format == 's')
		{
			++format;
			const char	*s = va_arg(parameters, const char*);
			print(s, strlen(s));
		}else if (*format == 'd')
		{
			++format;
			const int nb = va_arg(parameters, const int);
			dectoa(nb);
		}else if (*format == 'x')
		{
			++format;
			const int nb = va_arg(parameters, const int);
			hextoa(nb);
		}else
			goto incomprehensible_conversion;
	}
	va_end(parameters);
	return written;
}
