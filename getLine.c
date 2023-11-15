#include "custom_shell.h"

/**
 * custom_buffer_input - buffers chained commands.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @buffer: Address of the buffer.
 * @buffer_length: Address of the length variable.
 *
 * Return: Number of bytes read.
 */
ssize_t custom_buffer_input(info_t *info, char **buffer, size_t *buffer_length)
{
	ssize_t bytes_read = 0;
	size_t buffer_pos = 0;

	if (!*buffer_length) {
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, custom_sigint_handler);

		#if USE_GETLINE
		bytes_read = getline(buffer, &buffer_pos, stdin);
		#else
		bytes_read = custom_getline(info, buffer, &buffer_pos);
		#endif

		if (bytes_read > 0) {
			if ((*buffer)[bytes_read - 1] == '\n') {
				(*buffer)[bytes_read - 1] = '\0'; /* Remove trailing newline */
				bytes_read--;
			}
			info->linecount_flag = 1;
			custom_remove_comments(*buffer);
			custom_build_history_list(info, *buffer, info->histcount++);
			{
				*buffer_length = bytes_read;
				info->cmd_buf = buffer;
			}
		}
	}

	return bytes_read;
}

/**
 * custom_get_input - gets a line minus the newline.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: Number of bytes read.
 */
ssize_t custom_get_input(info_t *info)
{
	static char *buffer; /* The ';' command chain buffer */
	static size_t buffer_iterator, j, buffer_length;
	ssize_t bytes_read = 0;
	char **buffer_ptr = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	bytes_read = custom_buffer_input(info, &buffer, &buffer_length);

	if (bytes_read == -1) /* EOF */
		return -1;

	if (buffer_length) { /* We have commands left in the chain buffer */
		j = buffer_iterator; /* Init new iterator to the current buffer position */
		p = buffer + buffer_iterator; /* Get pointer for return */

		custom_check_chain(info, buffer, &j, buffer_iterator, buffer_length);
		while (j < buffer_length) { /* Iterate to semicolon or end */
			if (custom_is_chain(info, buffer, &j))
				break;
			j++;
		}

		buffer_iterator = j + 1; /* Increment past nulled ';'' */
		if (buffer_iterator >= buffer_length) { /* Reached the end of the buffer? */
			buffer_iterator = buffer_length = 0; /* Reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buffer_ptr = p; /* Pass back a pointer to the current command position */
		return custom_strlen(p); /* Return the length of the current command */
	}

	*buffer_ptr = buffer; /* Not a chain, so pass back the buffer from custom_getline() */
	return bytes_read; /* Return the length of the buffer from custom_getline() */
}

/**
 * custom_read_buffer - reads a buffer.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @buffer: Buffer.
 * @iterator: Size.
 *
 * Return: Number of bytes read.
 */
ssize_t custom_read_buffer(info_t *info, char *buffer, size_t *iterator)
{
	ssize_t bytes_read = 0;

	if (*iterator)
		return 0;

	bytes_read = read(info->readfd, buffer, READ_BUF_SIZE);
	if (bytes_read >= 0)
		*iterator = bytes_read;

	return bytes_read;
}

/**
 * custom_getline - gets the next line of input from STDIN.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @ptr: Address of a pointer to a buffer, preallocated or NULL.
 * @length: Size of the preallocated ptr buffer if not NULL.
 *
 * Return: The number of bytes read.
 */
int custom_getline(info_t *info, char **ptr, size_t *length)
{
	static char buffer[READ_BUF_SIZE];
	static size_t buffer_iterator, buffer_length;
	size_t k;
	ssize_t bytes_read = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		k = *length;
	if (buffer_iterator == buffer_length)
		buffer_iterator = buffer_length = 0;

	bytes_read = custom_read_buffer(info, buffer, &buffer_length);

	if (bytes_read == -1 || (bytes_read == 0 && buffer_length == 0))
		return -1;

	c = custom_strchr(buffer + buffer_iterator, '\n');
	k = c ? 1 + (unsigned int)(c - buffer) : buffer_length;
	new_p = custom_realloc(p, k, k ? k + buffer_length : buffer_length + 1);
	if (!new_p)
		return p ? free(p), -1 : -1;

	if (k)
		custom_strncat(new_p, buffer + buffer_iterator, k - buffer_iterator);
	else
		custom_strncpy(new_p, buffer + buffer_iterator, k - buffer_iterator + 1);

	k += buffer_iterator;
	buffer_iterator = k;
	p = new_p;

	if (length)
		*length = k;
	*ptr = p;
	return k;
}

/**
 * custom_sigint_handler - blocks ctrl-C.
 * @sig_num: The signal number.
 *
 * Return: Void.
 */
void custom_sigint_handler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

