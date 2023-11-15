#include "shell.h"

/**
 * custom_strlen - Returns the length of a string.
 * @str: The string whose length is to be determined.
 *
 * Return: The integer length of the string.
 */
int custom_strlen(char *str)
{
	int length = 0;

	if (!str)
		return (0);

	while (*str++)
		length++;

	return (length);
}

/**
 * custom_strcmp - Performs a lexicographic comparison of two strings.
 * @str1: The first string.
 * @str2: The second string.
 *
 * Return: A negative value if str1 < str2, a positive value if str1 > str2, 0 if str1 == str2.
 */
int custom_strcmp(char *str1, char *str2)
{
	while (*str1 && *str2)
	{
		if (*str1 != *str2)
			return (*str1 - *str2);
		str1++;
		str2++;
	}

	if (*str1 == *str2)
		return (0);
	else
		return (*str1 < *str2 ? -1 : 1);
}

/**
 * custom_starts_with - Checks if a string starts with a given substring.
 * @str: The string to search.
 * @prefix: The substring to find.
 *
 * Return: The address of the next character in the string after the prefix, or NULL if it doesn't start with the prefix.
 */
char *custom_starts_with(const char *str, const char *prefix)
{
	while (*prefix)
	{
		if (*prefix++ != *str++)
			return (NULL);
	}

	return ((char *)str);
}

/**
 * custom_strcat - Concatenates two strings.
 * @dest: The destination buffer.
 * @src: The source buffer.
 *
 * Return: A pointer to the destination buffer.
 */
char *custom_strcat(char *dest, char *src)
{
	char *result = dest;

	while (*dest)
	{
		dest++;
	}

	while (*src)
	{
		*dest++ = *src++;
	}

	*dest = *src;
	return result;
}

