#include "shell.h"

/**
 * free_data - frees data structure
 * @datash: data structure
 */
void free_data(shell_data_t *datash)
{
	unsigned int i;

	for (i = 0; datash->_environ[i]; i++)
	{
		free(datash->_environ[i]);
	}

	free(datash->_environ);
	free(datash->pid);
}

/**
 * set_data - Initialize data structure
 * @datash: data structure
 * @av: argument vector
 */
void set_data(shell_data_t *datash, char **av)
{
	unsigned int i;

	datash->av = av;
	datash->input = NULL;
	datash->args = NULL;
	datash->status = 0;
	datash->counter = 1;

	for (i = 0; environ[i]; i++)
		;

	datash->_environ = malloc(sizeof(char *) * (i + 1));

	for (i = 0; environ[i]; i++)
	{
		datash->_environ[i] = _strdup(environ[i]);
	}

	datash->_environ[i] = NULL;
	datash->pid = convert_integer_to_string(getpid());
}

/**
 * main - Entry point
 * @ac: argument count
 * @av: argument vector
 * Return: 0 on success.
 */
int main(int ac, char **av)
{
	shell_data_t datash;
	(void) ac;

	signal(SIGINT, handle_sigint);
	set_data(&datash, av);
	run_shell_loop(&datash);
	free_data(&datash);
	if (datash.status < 0)
		return (255);
	return (datash.status);
}
