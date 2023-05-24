#include "shell.h"

/**
 * free_data - frees data structure
 * @data: data structure
 */
void free_data(shell_data_t *data)
{
	unsigned int i;

	for (i = 0; data->_env[i]; i++)
	{
		free(data->_env[i]);
	}

	free(data->_env);
	free(data->pid);
}

/**
 * set_data - Initialize data structure
 * @data: data structure
 * @av: argument vector
 */
void set_data(shell_data_t *data, char **av)
{
	unsigned int i;

	data->av = av;
	data->input = NULL;
	data->args = NULL;
	data->status = 0;
	data->counter = 1;

	for (i = 0; environ[i]; i++)
		;

	data->_env = malloc(sizeof(char *) * (i + 1));

	for (i = 0; environ[i]; i++)
	{
		data->_env[i] = _strdup(environ[i]);
	}

	data->_env[i] = NULL;
	data->pid = int_to_string(getpid());
}

/**
 * main - Entry point
 * @ac: argument count
 * @av: argument vector
 * Return: 0 on success.
 */
int main(int ac, char **av)
{
	shell_data_t data;
	(void) ac;

	signal(SIGINT, handle_sigint);
	set_data(&data, av);
	run_shell_loop(&data);
	free_data(&data);
	if (data.status < 0)
		return (255);
	return (data.status);
}
