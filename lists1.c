#include "shell.h"

/**
 * custom_list_length - determines the length of a linked list.
 * @head: Pointer to the first node of the list.
 *
 * Return: Size of the list.
 */
size_t custom_list_length(const list_t *head)
{
	size_t count = 0;

	while (head)
	{
		head = head->next;
		count++;
	}
	return count;
}

/**
 * custom_list_to_strings - returns an array of strings from the list->str.
 * @head: Pointer to the first node of the list.
 *
 * Return: An array of strings.
 */
char **custom_list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t count = custom_list_length(head), index;
	char **strings;
	char *str;

	if (!head || !count)
		return NULL;

	strings = custom_malloc(sizeof(char *) * (count + 1));
	if (!strings)
		return NULL;

	for (index = 0; node; node = node->next, index++)
	{
		str = custom_malloc(custom_strlen(node->str) + 1);
		if (!str)
		{
			for (size_t j = 0; j < index; j++)
				custom_free(strings[j]);
			custom_free(strings);
			return NULL;
		}

		str = custom_strcpy(str, node->str);
		strings[index] = str;
	}
	strings[index] = NULL;

	return strings;
}

/**
 * custom_print_list - prints all elements of a list_t linked list.
 * @head: Pointer to the first node of the list.
 *
 * Return: Size of the list.
 */
size_t custom_print_list(const list_t *head)
{
	size_t count = 0;

	while (head)
	{
		custom_puts(custom_convert_number(head->num, 10, 0));
		custom_putchar(':');
		custom_putchar(' ');
		custom_puts(head->str ? head->str : "(nil)");
		custom_puts("\n");
		head = head->next;
		count++;
	}
	return count;
}

/**
 * custom_node_starts_with - returns the node whose string starts with a prefix.
 * @node: Pointer to the list head.
 * @prefix: String to match.
 * @c: The next character after the prefix to match.
 *
 * Return: Matching node or NULL.
 */
list_t *custom_node_starts_with(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = custom_starts_with(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return node;
		node = node->next;
	}
	return NULL;
}

/**
 * custom_get_node_index - gets the index of a node.
 * @head: Pointer to the list head.
 * @node: Pointer to the node.
 *
 * Return: Index of the node or -1.
 */
ssize_t custom_get_node_index(list_t *head, list_t *node)
{
	size_t index = 0;

	while (head)
	{
		if (head == node)
			return index;
		head = head->next;
		index++;
	}
	return -1;
}

