#include "shell.h"

/**
 * handle_sigint - Handle the crtl + c call in prompt
 * @sig: Signal handler
 */
void handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n^-^ ", 5);
}

/**
 * get_help - function that retrieves help messages according builtin
 * @data: data structure (args and input)
 * Return: Return 0
*/
int get_help(shell_data_t *data)
{

	if (data->args[1] == 0)
		print_help_general();
	else if (_strcmp(data->args[1], "setenv") == 0)
		print_help_setenv();
	else if (_strcmp(data->args[1], "env") == 0)
		print_help_env();
	else if (_strcmp(data->args[1], "unsetenv") == 0)
		print_help_unsetenv();
	else if (_strcmp(data->args[1], "help") == 0)
		print_help();
	else if (_strcmp(data->args[1], "exit") == 0)
		print_help_exit();
	else if (_strcmp(data->args[1], "cd") == 0)
		print_help_cd();
	else if (_strcmp(data->args[1], "alias") == 0)
		print_help_alias();
	else
		write(STDERR_FILENO, data->args[0],
		      _strlen(data->args[0]));

	data->status = 0;
	return (1);
}

/**
 * get_error_code - calls the error according the builtin, syntax or permission
 * @data: data structure that contains arguments
 * @eval: error value
 * Return: error
 */
int get_error_code(shell_data_t *data, int eval)
{
	char *error;

	switch (eval)
	{
	case -1:
		error = error_message_env(data);
		break;
	case 126:
		error = error_message_path_126(data);
		break;
	case 127:
		error = error_message_not_found(data);
		break;
	case 2:
		if (_strcmp("exit", data->args[0]) == 0)
			error = error_message_exit_shell(data);
		else if (_strcmp("cd", data->args[0]) == 0)
			error = error_message_get_cd(data);
		break;
	}

	if (error)
	{
		write(STDERR_FILENO, error, _strlen(error));
		free(error);
	}

	data->status = eval;
	return (eval);
}

/**
 * get_builtin_function - builtin that pais the command in the arg
 * @cmd: command
 * Return: function pointer of the builtin command
 */
int (*get_builtin_function(char *cmd))(shell_data_t *)
{
	builtin_t builtin[] = {
		{ "env", print_env_var },
		{ "exit", exit_shell_program },
		{ "setenv", _setenv },
		{ "unsetenv", _unsetenv },
		{ "cd", change_directory_shell },
		{ "help", get_help },
		{ NULL, NULL }
	};
	int i;

	for (i = 0; builtin[i].name; i++)
	{
		if (_strcmp(builtin[i].name, cmd) == 0)
			break;
	}

	return (builtin[i].f);
}

/**
 * exit_shell_program - exits the shell
 * @data: data relevant (status and args)
 * Return: 0 on success.
 */
int exit_shell_program(shell_data_t *data)
{
	unsigned int ustatus;
	int is_digit;
	int str_len;
	int big_number;

	if (data->args[1] != NULL)
	{
		ustatus = str_to_int(data->args[1]);
		is_digit = _isdigit(data->args[1]);
		str_len = _strlen(data->args[1]);
		big_number = ustatus > (unsigned int)INT_MAX;
		if (!is_digit || str_len > 10 || big_number)
		{
			get_error_code(data, 2);
			data->status = 2;
			return (1);
		}
		data->status = (ustatus % 256);
	}
	return (0);
}
