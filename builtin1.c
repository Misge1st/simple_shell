#include "shell.h"

/**
 * _myhistory - Displays the command history list with line numbers.
 * @info: Structure containing potential arguments, maintaining
 *        a constant function prototype.
 * Return: Always 0.
 */
int _myhistory(info_t *info)
{
    // Calls a function to print the history list with line numbers.
    print_list(info->history);
    return (0);
}

/**
 * unset_alias - Removes an alias from the alias list.
 * @info: Parameter struct.
 * @str: The alias string to be removed.
 * Return: Returns 0 on success, 1 on error.
 */
int unset_alias(info_t *info, char *str)
{
    char *p, c;
    int ret;

    // Searches for the '=' character in the alias string.
    p = _strchr(str, '=');
    if (!p)
        return (1);

    // Temporarily replaces '=' with null terminator to extract the alias name.
    c = *p;
    *p = 0;

    // Deletes the alias node from the alias list.
    ret = delete_node_at_index(&(info->alias),
                                get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
    *p = c;

    return (ret);
}

/**
 * set_alias - Adds or updates an alias in the alias list.
 * @info: Parameter struct.
 * @str: The alias string to be added or updated.
 * Return: Returns 0 on success, 1 on error.
 */
int set_alias(info_t *info, char *str)
{
    char *p;

    // Searches for the '=' character in the alias string.
    p = _strchr(str, '=');
    if (!p)
        return (1);

    // If the string after '=' is empty, it removes the alias.
    if (!*++p)
        return (unset_alias(info, str));

    // Removes any existing alias with the same name.
    unset_alias(info, str);

    // Adds the new alias to the alias list.
    return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - Prints an alias string.
 * @node: The alias node to be printed.
 * Return: Returns 0 on success, 1 on error.
 */
int print_alias(list_t *node)
{
    char *p = NULL, *a = NULL;

    // Prints the alias name and value enclosed in single quotes.
    if (node)
    {
        p = _strchr(node->str, '=');
        for (a = node->str; a <= p; a++)
            _putchar(*a);
        _putchar('\'');
        _puts(p + 1);
        _puts("'\n");
        return (0);
    }
    return (1);
}

/**
 * _myalias - Mimics the behavior of the 'alias' shell builtin command.
 * @info: Structure containing potential arguments.
 * Return: Always returns 0.
 */
int _myalias(info_t *info)
{
    int i = 0;
    char *p = NULL;
    list_t *node = NULL;

    // If no arguments provided, prints all aliases.
    if (info->argc == 1)
    {
        node = info->alias;
        while (node)
        {
            print_alias(node);
            node = node->next;
        }
        return (0);
    }

    // Processes each argument.
    for (i = 1; info->argv[i]; i++)
    {
        // If an argument contains '=', adds or updates an alias.
        p = _strchr(info->argv[i], '=');
        if (p)
            set_alias(info, info->argv[i]);
        else
            // If no '=', prints the alias associated with the provided name.
            print_alias(node_starts_with(info->alias, info->argv[i], '='));
    }

    return (0);
}

