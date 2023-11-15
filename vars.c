#include "shell.h"

/**
 * is_chain_delimiter - Tests if the current character in the buffer is a chain delimiter.
 * @info: The parameter struct.
 * @buf: The character buffer.
 * @p: Address of the current position in buf.
 *
 * Return: 1 if it's a chain delimiter, 0 otherwise.
 */
int is_chain_delimiter(info_t *info, char *buf, size_t *p)
{
	size_t current_pos = *p;

	if (buf[current_pos] == '|' && buf[current_pos + 1] == '|')
	{
		buf[current_pos] = 0;
		current_pos++;
		info->cmd_buf_type = CMD_OR;
	}
	else if (buf[current_pos] == '&' && buf[current_pos + 1] == '&')
	{
		buf[current_pos] = 0;
		current_pos++;
		info->cmd_buf_type = CMD_AND;
	}
	else if (buf[current_pos] == ';') /* Found end of this command */
	{
		buf[current_pos] = 0; /* Replace semicolon with null */
		info->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);

	*p = current_pos;
	return (1);
}

/**
 * check_chain_continue - Checks whether we should continue chaining based on the last status.
 * @info: The parameter struct.
 * @buf: The character buffer.
 * @p: Address of the current position in buf.
 * @start_pos: Starting position in buf.
 * @len: Length of buf.
 *
 * Return: Void.
 */
void check_chain_continue(info_t *info, char *buf, size_t *p, size_t start_pos, size_t len)
{
	size_t current_pos = *p;

	if (info->cmd_buf_type == CMD_AND)
	{
		if (info->status)
		{
			buf[start_pos] = 0;
			current_pos = len;
		}
	}
	if (info->cmd_buf_type == CMD_OR)
	{
		if (!info->status)
		{
			buf[start_pos] = 0;
			current_pos = len;
		}
	}

	*p = current_pos;
}

/**
 * replace_aliases - Replaces aliases in the tokenized string.
 * @info: The parameter struct.
 *
 * Return: 1 if replaced, 0 otherwise.
 */
int replace_aliases(info_t *info)
{
	int i;
	list_t *alias_node;
	char *alias_value;

	for (i = 0; i < 10; i++)
	{
		alias_node = node_starts_with(info->alias, info->argv[0], '=');
		if (!alias_node)
			return (0);

		free(info->argv[0]);

		alias_value = _strchr(alias_node->str, '=');
		if (!alias_value)
			return (0);

		alias_value = _strdup(alias_value + 1);
		if (!alias_value)
			return (0);

		info->argv[0] = alias_value;
	}

	return (1);
}

/**
 * replace_variables - Replaces variables in the tokenized string.
 * @info: The parameter struct.
 *
 * Return: 1 if replaced, 0 otherwise.
 */
int replace_variables(info_t *info)
{
	int i = 0;
	list_t *env_node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!_strcmp(info->argv[i], "$?"))
		{
			replace_string(&(info->argv[i]), _strdup(convert_number(info->status, 10, 0)));
			continue;
		}
		if (!_strcmp(info->argv[i], "$$"))
		{
			replace_string(&(info->argv[i]), _strdup(convert_number(getpid(), 10, 0)));
			continue;
		}

		env_node = node_starts_with(info->env, &info->argv[i][1], '=');
		if (env_node)
		{
			replace_string(&(info->argv[i]), _strdup(_strchr(env_node->str, '=') + 1));
			continue;
		}

		replace_string(&info->argv[i], _strdup(""));
	}

	return (1);
}

/**
 * replace_string - Replaces a string.
 * @old: Address of the old string.
 * @new: New string.
 *
 * Return: 1 if replaced, 0 otherwise.
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}

