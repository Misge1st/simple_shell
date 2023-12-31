#include "custom_shell.h"

/**
 * custom_memset - Fills memory with a constant byte.
 * @s: The pointer to the memory area.
 * @b: The byte to fill *s with.
 * @n: The amount of bytes to be filled.
 *
 * Return: Pointer to the memory area s.
 */
char *custom_memset(char *s, char b, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		s[i] = b;
	return s;
}

/**
 * custom_ffree - Frees a string of strings.
 * @pp: String of strings.
 */
void custom_ffree(char **pp)
{
	char **a = pp;

	if (!pp)
		return;
	while (*pp)
		custom_free(*pp++);
	custom_free(a);
}

/**
 * custom_realloc - Reallocates a block of memory.
 * @ptr: Pointer to the previous malloc'ated block.
 * @old_size: Byte size of the previous block.
 * @new_size: Byte size of the new block.
 *
 * Return: Pointer to the old block.
 */
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p;

	if (!ptr)
		return custom_malloc(new_size);
	if (!new_size)
		return (custom_free(ptr), NULL);
	if (new_size == old_size)
		return ptr;

	p = custom_malloc(new_size);
	if (!p)
		return NULL;

	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		p[old_size] = ((char *)ptr)[old_size];
	custom_free(ptr);
	return p;
}
