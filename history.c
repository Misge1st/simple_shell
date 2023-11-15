#include "custom_shell.h"

/**
 * custom_get_history_file - retrieves the history file.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: Allocated string containing the history file.
 */
char *custom_get_history_file(info_t *info)
{
	char *file_path, *home_directory;

	home_directory = custom_getenv(info, "HOME=");
	if (!home_directory)
		return NULL;

	file_path = custom_malloc(sizeof(char) * (custom_strlen(home_directory) + custom_strlen(HISTORY_FILENAME) + 2));
	if (!file_path)
		return NULL;

	file_path[0] = 0;
	custom_strcpy(file_path, home_directory);
	custom_strcat(file_path, "/");
	custom_strcat(file_path, HISTORY_FILENAME);

	return file_path;
}

/**
 * custom_write_history - creates a file or appends to an existing file.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: 1 on success, -1 on failure.
 */
int custom_write_history(info_t *info)
{
	ssize_t file_descriptor;
	char *filename = custom_get_history_file(info);
	list_t *node = NULL;

	if (!filename)
		return -1;

	file_descriptor = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	custom_free(filename);

	if (file_descriptor == -1)
		return -1;

	for (node = info->history; node; node = node->next) {
		custom_putsfd(node->str, file_descriptor);
		custom_putfd('\n', file_descriptor);
	}

	custom_putfd(BUF_FLUSH, file_descriptor);
	close(file_descriptor);

	return 1;
}

/**
 * custom_read_history - reads history from a file.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: History count on success, 0 on failure.
 */
int custom_read_history(info_t *info)
{
	int i, last = 0, history_count = 0;
	ssize_t file_descriptor, read_length, file_size = 0;
	struct stat st;
	char *buffer = NULL, *filename = custom_get_history_file(info);

	if (!filename)
		return 0;

	file_descriptor = open(filename, O_RDONLY);
	custom_free(filename);

	if (file_descriptor == -1)
		return 0;

	if (!fstat(file_descriptor, &st))
		file_size = st.st_size;

	if (file_size < 2)
		return 0;

	buffer = custom_malloc(sizeof(char) * (file_size + 1));
	if (!buffer)
		return 0;

	read_length = read(file_descriptor, buffer, file_size);
	buffer[file_size] = 0;

	if (read_length <= 0)
		return custom_free(buffer), 0;

	close(file_descriptor);

	for (i = 0; i < file_size; i++) {
		if (buffer[i] == '\n') {
			buffer[i] = 0;
			custom_build_history_list(info, buffer + last, history_count++);
			last = i + 1;
		}
	}

	if (last != i)
		custom_build_history_list(info, buffer + last, history_count++);

	custom_free(buffer);
	info->histcount = history_count;

	while (info->histcount-- >= HISTORY_MAX)
		custom_delete_node_at_index(&(info->history), 0);

	custom_renumber_history(info);
	return info->histcount;
}

/**
 * custom_build_history_list - adds an entry to a history linked list.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @buffer: Buffer.
 * @linecount: The history line count (histcount).
 *
 * Return: Always 0.
 */
int custom_build_history_list(info_t *info, char *buffer, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;

	custom_add_node_end(&node, buffer, linecount);

	if (!info->history)
		info->history = node;

	return 0;
}

/**
 * custom_renumber_history - renumbers the history linked list after changes.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: The new histcount.
 */
int custom_renumber_history(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node) {
		node->num = i++;
		node = node->next;
	}

	return info->histcount = i;
}

