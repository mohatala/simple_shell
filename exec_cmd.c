#include "shell.h"

/**
 * _current_dir - this function checks if :
 *				is in the current directory.
 * @path: type char pointer char.
 * @i: type int pointer of index.
 * Return: 1 if the path is searchable in the cd, 0 otherwise.
 */
int _current_dir(char *path, int *i)
{
	if (path[*i] == ':')
		return (1);

	while (path[*i] != ':' && path[*i])
	{
		*i += 1;
	}

	if (path[*i])
		*i += 1;

	return (0);
}

/**
 * find_cmd - this function locates a command
 * @cmd: command name
 * @_environ: environment variable
 * Return: location of the command.
 */
char *find_cmd(char *cmd, char **_environ)
{
	char *path, *ptr_path, *token_path, *dir;
	int len_dir, len_cmd, i;
	struct stat st;

	path = get_env("PATH", _environ);
	if (path)
	{
		ptr_path = _strdup(path);
		len_cmd = _strlen(cmd);
		token_path = _str_del(ptr_path, ":");
		i = 0;
		while (token_path != NULL)
		{
			if (_current_dir(path, &i))
				if (stat(cmd, &st) == 0)
					return (cmd);
			len_dir = _strlen(token_path);
			dir = malloc(len_dir + len_cmd + 2);
			_strcpy(dir, token_path);
			_strcat(dir, "/");
			_strcat(dir, cmd);
			_strcat(dir, "\0");
			if (stat(dir, &st) == 0)
			{
				free(ptr_path);
				return (dir);
			}
			free(dir);
			token_path = _str_del(NULL, ":");
		}
		free(ptr_path);
		if (stat(cmd, &st) == 0)
			return (cmd);
		return (NULL);
	}
	if (cmd[0] == '/')
		if (stat(cmd, &st) == 0)
			return (cmd);
	return (NULL);
}

/**
 * cmd_exec - this function determines if a command is executable
 * @data: data structure
 * Return: 0 if is not an executable, other number if it does
 */
int cmd_exec(shell_data_t *data)
{
	struct stat st;
	int i;
	char *input;

	input = data->args[0];
	for (i = 0; input[i]; i++)
	{
		if (input[i] == '.')
		{
			if (input[i + 1] == '.')
				return (0);
			if (input[i + 1] == '/')
				continue;
			else
				break;
		}
		else if (input[i] == '/' && i != 0)
		{
			if (input[i + 1] == '.')
				continue;
			i++;
			break;
		}
		else
			break;
	}
	if (i == 0)
		return (0);

	if (stat(input + i, &st) == 0)
	{
		return (i);
	}
	get_error_code(data, 127);
	return (-1);
}

/**
 * check_cmd_err - this function verifies if a user
 *				has permissions to access
 * @dir: destination directory
 * @data: data structure
 * Return: 1 if there is an error, 0 if not
 */
int check_cmd_err(char *dir, shell_data_t *data)
{
	if (dir == NULL)
	{
		get_error_code(data, 127);
		return (1);
	}

	if (_strcmp(data->args[0], dir) != 0)
	{
		if (access(dir, X_OK) == -1)
		{
			get_error_code(data, 126);
			free(dir);
			return (1);
		}
		free(dir);
	}
	else
	{
		if (access(data->args[0], X_OK) == -1)
		{
			get_error_code(data, 126);
			return (1);
		}
	}

	return (0);
}

/**
 * exec_cmmd - this function executes command lines
 * @data: data relevant (args and input)
 * Return: 1 on success.
 */
int exec_cmmd(shell_data_t *data)
{
	pid_t pd;
	pid_t wpd;
	int state;
	int exec;
	char *dir;
	(void) wpd;

	exec = cmd_exec(data);
	if (exec == -1)
		return (1);
	if (exec == 0)
	{
		dir = find_cmd(data->args[0], data->_env);
		if (check_cmd_err(dir, data) == 1)
			return (1);
	}

	pd = fork();
	if (pd == 0)
	{
		if (exec == 0)
			dir = find_cmd(data->args[0], data->_env);
		else
			dir = data->args[0];
		execve(dir + exec, data->args, data->_env);
	}
	else if (pd < 0)
	{
		perror(data->av[0]);
		return (1);
	}
	else
	{
		do {
			wpd = waitpid(pd, &state, WUNTRACED);
		} while (!WIFEXITED(state) && !WIFSIGNALED(state));
	}

	data->status = state / 256;
	return (1);
}
