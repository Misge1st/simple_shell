#include "custom_shell.h"

/**
 * custom_get_environment - Returns a copy of the environment as a string array.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: Always 0.
 */
char **custom_get_environment(info_t *info)
{
	if (!info->environment || info->environment_changed)
	{
		info->environment = custom_list_to_strings(info->environment_list);
		info->environment_changed = 0;
	}

	return info->environment;
}

/**
 * custom_unset_environment_variable - Remove an environment variable.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @variable: The environment variable to unset.
 *
 * Return: 1 if the variable is deleted, 0 otherwise.
 */
int custom_unset_environment_variable(info_t *info, char *variable)
{
	list_t *node = info->environment_list;
	size_t i = 0;
	char *p;

	if (!node || !variable)
		return 0;

	while (node)
	{
		p = custom_starts_with(node->str, variable);
		if (p && *p == '=')
		{
			info->environment_changed = custom_delete_node_at_index(&(info->environment_list), i);
			i = 0;
			node = info->environment_list;
			continue;
		}
		node = node->next;
		i++;
	}
	return info->environment_changed;
}

/**
 * custom_set_environment_variable - Initialize a new environment variable or modify an existing one.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @variable: The environment variable property.
 * @value: The environment variable value.
 *
 * Return: Always 0.
 */
int custom_set_environment_variable(info_t *info, char *variable, char *value)
{
	char *buffer = NULL;
	list_t *node;
	char *p;

	if (!variable || !value)
		return 0;

	buffer = malloc(custom_strlen(variable) + custom_strlen(value) + 2);
	if (!buffer)
		return 1;
	custom_strcpy(buffer, variable);
	custom_strcat(buffer, "=");
	custom_strcat(buffer, value);
	node = info->environment_list;
	while (node)
	{
		p = custom_starts_with(node->str, variable);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buffer;
			info->environment_changed = 1;
			return 0;
		}
		node = node->next;
	}
	custom_add_node_end(&(info->environment_list), buffer, 0);
	free(buffer);
	info->environment_changed = 1;
	return 0;
}

