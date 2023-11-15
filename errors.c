#include "custom_shell.h"

/**
 * custom_puts_stderr - Prints a string to stderr.
 * @str: The string to be printed
 *
 * Return: Nothing
 */
void custom_puts_stderr(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		custom_putchar_stderr(str[i]);
		i++;
	}
}

/**
 * custom_putchar_stderr - Writes a character to stderr.
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int custom_putchar_stderr(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return 1;
}

/**
 * custom_putchar_to_fd - Writes a character to a given file descriptor.
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int custom_putchar_to_fd(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return 1;
}

/**
 * custom_puts_to_fd - Prints an input string to a given file descriptor.
 * @str: The string to be printed
 * @fd: The file descriptor to write to
 *
 * Return: The number of characters printed.
 */
int custom_puts_to_fd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return 0;
	while (*str)
	{
		i += custom_putchar_to_fd(*str++, fd);
	}
	return i;
}

