#include "shell.h"

/**
 * compare_environement_variable_name - this function compares
 *					environment variables names
 * with the name passed.
 * @nenv: name of the environment variable
 * @name: name passed
 * Return: 0 if are not equal. Another value if they are.
 */
int compare_environement_variable_name(const char *nenv, const char *name)
{
	int i;

	for (i = 0; nenv[i] != '='; i++)
	{
		if (nenv[i] != name[i])
		{
			return (0);
		}
	}

	return (i + 1);
}

/**
 * copy_info - copies info to create
 * a new env or alias
 * @name: name (env or alias)
 * @value: value (env or alias)
 *
 * Return: new env or alias.
 */
char *copy_info(char *name, char *value)
{
	char *new;
	int len_name, len_value, len;

	len_name = _strlen(name);
	len_value = _strlen(value);
	len = len_name + len_value + 2;
	new = malloc(sizeof(char) * (len));
	_strcpy(new, name);
	_strcat(new, "=");
	_strcat(new, value);
	_strcat(new, "\0");

	return (new);
}

/**
 * set_env - sets an environment variable
 * @name: name of the environment variable
 * @value: value of the environment variable
 * @data: data structure (environ)
 * Return: no return
 */
void set_env(char *name, char *value, shell_data_t *data)
{
	int i;
	char *var_env, *name_env;

	for (i = 0; data->_env[i]; i++)
	{
		var_env = _strdup(data->_env[i]);
		name_env = _str_del(var_env, "=");
		if (_strcmp(name_env, name) == 0)
		{
			free(data->_env[i]);
			data->_env[i] = copy_info(name_env, value);
			free(var_env);
			return;
		}
		free(var_env);
	}

	data->_env = _reallocdp(data->_env, i, sizeof(char *) * (i + 2));
	data->_env[i] = copy_info(name, value);
	data->_env[i + 1] = NULL;
}

/**
 * _setenv - compares env variables names
 * with the name passed.
 * @data: data relevant (env name and env value)
 * Return: 1 on success.
 */
int _setenv(shell_data_t *data)
{

	if (data->args[1] == NULL || data->args[2] == NULL)
	{
		get_error_code(data, -1);
		return (1);
	}

	set_env(data->args[1], data->args[2], data);

	return (1);
}

/**
 * _unsetenv - deletes a environment variable
 * @data: data relevant (env name)
 * Return: 1 on success.
 */
int _unsetenv(shell_data_t *data)
{
	char **realloc_env;
	char *var_env, *name_env;
	int i, j, k;

	if (data->args[1] == NULL)
	{
		get_error_code(data, -1);
		return (1);
	}
	k = -1;
	for (i = 0; data->_env[i]; i++)
	{
		var_env = _strdup(data->_env[i]);
		name_env = _str_del(var_env, "=");
		if (_strcmp(name_env, data->args[1]) == 0)
		{
			k = i;
		}
		free(var_env);
	}
	if (k == -1)
	{
		get_error_code(data, -1);
		return (1);
	}
	realloc_env = malloc(sizeof(char *) * (i));
	for (i = j = 0; data->_env[i]; i++)
	{
		if (i != k)
		{
			realloc_env[j] = data->_env[i];
			j++;
		}
	}
	realloc_env[j] = NULL;
	free(data->_env[k]);
	free(data->_env);
	data->_env = realloc_env;
	return (1);
}
