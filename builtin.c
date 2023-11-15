#include "shell.h"

/**
 * custom_exit - Exits the custom shell
 * @info: Structure containing potential arguments
 *
 * Return: Exits with a given exit status (0) if info->arguments[0] != "exit"
 */
int custom_exit(info_t *info)
{
	int exit_status;

	if (info->arguments[1]) /* If there is an exit argument */
	{
		exit_status = custom_str_to_int(info->arguments[1]);
		if (exit_status == -1)
		{
			info->status = 2;
			print_custom_error(info, "Illegal number: ");
			custom_puts(info->arguments[1]);
			custom_putchar('\n');
			return 1;
		}
		info->error_number = custom_str_to_int(info->arguments[1]);
		return -2;
	}
	info->error_number = -1;
	return -2;
}

/**
 * custom_change_directory - Changes the current directory of the custom shell process
 * @info: Structure containing potential arguments
 *
 * Return: Always 0
 */
int custom_change_directory(info_t *info)
{
	char *cwd, *dir, buffer[1024];
	int chdir_result;

	cwd = getcwd(buffer, 1024);
	if (!cwd)
		custom_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->arguments[1])
	{
		dir = custom_getenv(info, "HOME=");
		if (!dir)
			chdir_result = /* TODO: what should this be? */
				chdir((dir = custom_getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_result = chdir(dir);
	}
	else if (custom_strcmp(info->arguments[1], "-") == 0)
	{
		if (!custom_getenv(info, "OLDPWD="))
		{
			custom_puts(cwd);
			custom_putchar('\n');
			return 1;
		}
		custom_puts(custom_getenv(info, "OLDPWD=")), custom_putchar('\n');
		chdir_result = /* TODO: what should this be? */
			chdir((dir = custom_getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_result = chdir(info->arguments[1]);
	if (chdir_result == -1)
	{
		print_custom_error(info, "can't cd to ");
		custom_eputs(info->arguments[1]), custom_epuchar('\n');
	}
	else
	{
		custom_setenv(info, "OLDPWD", custom_getenv(info, "PWD="));
		custom_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return 0;
}

/**
 * custom_help - Displays help information for the custom shell
 * @info: Structure containing potential arguments
 *
 * Return: Always 0
 */
int custom_help(info_t *info)
{
	char **arg_array;

	arg_array = info->arguments;
	custom_puts("Help call works. Function not yet implemented\n");
	if (0)
		custom_puts(*arg_array); /* Temp att_unused workaround */
	return 0;
}

