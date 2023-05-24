#include "shell.h"

/**
 * get_env - this fucntion gets an environment variable
 * @name: name of the environment variable
 * @_env: environment variable
 * Return: value of the environment variable if is found.
 *		In other case, returns NULL.
 */
char *get_env(const char *name, char **_env)
{
	char *ptr_env;
	int i, k;

	/* Initialize ptr_env value */
	ptr_env = NULL;
	k = 0;
	/* Comparing all environment variables */
	for (i = 0; _env[i]; i++)
	{
		k = comp_env_varname(_env[i], name);
		if (k)
		{
			ptr_env = _env[i];
			break;
		}
	}

	return (ptr_env + k);
}

/**
 * print_env_var - prints the evironment variables
 * @data: data relevant.
 * Return: 1 on success.
 */
int print_env_var(data_sh *data)
{
	int i, j;

	for (i = 0; data->_env[i]; i++)
	{

		for (j = 0; data->_env[i][j]; j++)
			;

		write(STDOUT_FILENO, data->_env[i], j);
		write(STDOUT_FILENO, "\n", 1);
	}
	data->status = 0;

	return (1);
}

/**
 * get_len_int - this function gets the lenght of a number.
 * @n: type int number.
 * Return: Lenght of a number.
 */
int get_len_int(int n)
{
	unsigned int n1;
	int len = 1;

	if (n < 0)
	{
		len++;
		n1 = n * -1;
	}
	else
	{
		n1 = n;
	}
	while (n1 > 9)
	{
		len++;
		n1 = n1 / 10;
	}

	return (len);
}
/**
 * int_to_string - this function converts int to string.
 * @n: type int number
 * Return: String.
 */
char *int_to_string(int n)
{
	unsigned int n1;
	int len = get_len_int(n);
	char *buff;

	buff = malloc(sizeof(char) * (len + 1));
	if (buff == 0)
		return (NULL);

	*(buff + len) = '\0';

	if (n < 0)
	{
		n1 = n * -1;
		buff[0] = '-';
	}
	else
	{
		n1 = n;
	}

	len--;
	do
	{
		*(buff + len) = (n1 % 10) + '0';
		n1 = n1 / 10;
		len--;
	} while (n1 > 0);
	return (buff);
}

/**
 * str_to_int - this function converts a string to an integer.
 * @s: input string.
 * Return: integer.
 */
int str_to_int(char *s)
{
	unsigned int counter = 0, size = 0, oi = 0, pn = 1, m = 1, i;

	while (*(s + counter) != '\0')
	{
		if (size > 0 && (*(s + counter) < '0' || *(s + counter) > '9'))
			break;

		if (*(s + counter) == '-')
			pn *= -1;

		if ((*(s + counter) >= '0') && (*(s + counter) <= '9'))
		{
			if (size > 0)
				m *= 10;
			size++;
		}
		counter++;
	}

	for (i = counter - size; i < counter; i++)
	{
		oi = oi + ((*(s + i) - 48) * m);
		m /= 10;
	}
	return (oi * pn);
}
