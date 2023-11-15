#include "shell.h"

/**
 * custom_str_to_int - Converts a string to an integer.
 * @s: The string to be converted.
 *
 * Return: The converted integer if successful, -1 on error.
 */
int custom_str_to_int(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;
	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return -1;
		}
		else
			return -1;
	}
	return result;
}

/**
 * custom_print_custom_error - Prints a custom error message.
 * @info: The parameter & return info struct.
 * @error_string: String containing specified error type.
 *
 * Return: Nothing.
 */
void custom_print_custom_error(info_t *info, char *error_string)
{
	custom_puts_stderr(info->fname);
	custom_puts_stderr(": ");
	custom_print_d(info->line_count, STDERR_FILENO);
	custom_puts_stderr(": ");
	custom_puts_stderr(info->argv[0]);
	custom_puts_stderr(": ");
	custom_puts_stderr(error_string);
}

/**
 * custom_print_d - Function prints a decimal (integer) number (base 10).
 * @input: The input.
 * @fd: The file descriptor to write to.
 *
 * Return: Number of characters printed.
 */
int custom_print_d(int input, int fd)
{
	int (*custom_putchar_func)(char) = custom_putchar;
	int i, count = 0;
	unsigned int abs_value, current;

	if (fd == STDERR_FILENO)
		custom_putchar_func = custom_putchar;
	if (input < 0)
	{
		abs_value = -input;
		custom_putchar_func('-');
		count++;
	}
	else
		abs_value = input;
	current = abs_value;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (abs_value / i)
		{
			custom_putchar_func('0' + current / i);
			count++;
		}
		current %= i;
	}
	custom_putchar_func('0' + current);
	count++;

	return count;
}

/**
 * custom_convert_number - Converter function, a clone of itoa.
 * @num: Number.
 * @base: Base.
 * @flags: Argument flags.
 *
 * Return: String.
 */
char *custom_convert_number(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CUSTOM_CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';
	}
	array = flags & CUSTOM_CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return ptr;
}

/**
 * custom_remove_comments - Function replaces the first instance of '#' with '\0'.
 * @buf: Address of the string to modify.
 *
 * Return: Always 0.
 */
void custom_remove_comments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}

