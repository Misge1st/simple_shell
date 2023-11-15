#include "shell.h"

/**
 * custom_strcpy - Copies a source string to a destination string.
 * @destination: The destination string.
 * @source: The source string.
 *
 * Return: A pointer to the destination string.
 */
char *custom_strcpy(char *destination, char *source)
{
	int i = 0;

	if (destination == source || source == 0)
		return (destination);

	while (source[i])
	{
		destination[i] = source[i];
		i++;
	}

	destination[i] = 0;
	return destination;
}

/**
 * custom_strdup - Duplicates a string.
 * @str: The string to duplicate.
 *
 * Return: A pointer to the duplicated string.
 */
char *custom_strdup(const char *str)
{
	int length = 0;
	char *result;

	if (str == NULL)
		return (NULL);

	while (*str++)
		length++;

	result = malloc(sizeof(char) * (length + 1));
	if (!result)
		return (NULL);

	for (length++; length--;)
		result[length] = *--str;

	return result;
}

/**
 * custom_puts - Prints an input string.
 * @str: The string to be printed.
 *
 * Return: Nothing.
 */
void custom_puts(char *str)
{
	int i = 0;

	if (!str)
		return;

	while (str[i] != '\0')
	{
		custom_putchar(str[i]);
		i++;
	}
}

/**
 * custom_putchar - Writes the character c to stdout.
 * @c: The character to print.
 *
 * Return: On success, it returns 1.
 * On error, it returns -1, and sets errno appropriately.
 */
int custom_putchar(char c)
{
	static int i;
	static char buffer[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(1, buffer, i);
		i = 0;
	}

	if (c != BUF_FLUSH)
		buffer[i++] = c;

	return 1;
}

