#include "shell.h"

/**
 * change_to_dot_dir - this function changes to the parent directory
 * @data: data relevant (environ)
 */
void change_to_dot_dir(shell_data_t *data)
{
	char pwd[PATH_MAX];
	char *dir, *cp_pwd, *cp_str_del_pwd;

	getcwd(pwd, sizeof(pwd));
	cp_pwd = _strdup(pwd);
	set_env("OLDPWD", cp_pwd, data);
	dir = data->args[1];
	if (_strcmp(".", dir) == 0)
	{
		set_env("PWD", cp_pwd, data);
		free(cp_pwd);
		return;
	}
	if (_strcmp("/", cp_pwd) == 0)
	{
		free(cp_pwd);
		return;
	}
	cp_str_del_pwd = cp_pwd;
	str_rev(cp_str_del_pwd);
	cp_str_del_pwd = _str_del(cp_str_del_pwd, "/");
	if (cp_str_del_pwd != NULL)
	{
		cp_str_del_pwd = _str_del(NULL, "\0");

		if (cp_str_del_pwd != NULL)
			str_rev(cp_str_del_pwd);
	}
	if (cp_str_del_pwd != NULL)
	{
		chdir(cp_str_del_pwd);
		set_env("PWD", cp_str_del_pwd, data);
	}
	else
	{
		chdir("/");
		set_env("PWD", "/", data);
	}
	data->status = 0;
	free(cp_pwd);
}

/**
 * change_to_dir - this function changes to a directory given by the user
 * @data: data relevant (directories)
 */
void change_to_dir(shell_data_t *data)
{
	char pwd[PATH_MAX];
	char *dir, *cp_pwd, *cp_dir;

	getcwd(pwd, sizeof(pwd));

	dir = data->args[1];
	if (chdir(dir) == -1)
	{
		get_error_code(data, 2);
		return;
	}

	cp_pwd = _strdup(pwd);
	set_env("OLDPWD", cp_pwd, data);

	cp_dir = _strdup(dir);
	set_env("PWD", cp_dir, data);

	free(cp_pwd);
	free(cp_dir);

	data->status = 0;

	chdir(dir);
}

/**
 * change_to_ols_dir - this function changes
 *					to the previous directory
 * @data: data relevant (environ)
 */
void change_to_ols_dir(shell_data_t *data)
{
	char pwd[PATH_MAX];
	char *p_pwd, *p_oldpwd, *cp_pwd, *cp_oldpwd;

	getcwd(pwd, sizeof(pwd));
	cp_pwd = _strdup(pwd);

	p_oldpwd = get_env("OLDPWD", data->_env);

	if (p_oldpwd == NULL)
		cp_oldpwd = cp_pwd;
	else
		cp_oldpwd = _strdup(p_oldpwd);

	set_env("OLDPWD", cp_pwd, data);

	if (chdir(cp_oldpwd) == -1)
		set_env("PWD", cp_pwd, data);
	else
		set_env("PWD", cp_oldpwd, data);

	p_pwd = get_env("PWD", data->_env);

	write(STDOUT_FILENO, p_pwd, _strlen(p_pwd));
	write(STDOUT_FILENO, "\n", 1);

	free(cp_pwd);
	if (p_oldpwd)
		free(cp_oldpwd);

	data->status = 0;

	chdir(p_pwd);
}

/**
 * change_to_home_dir - this function changes to home directory
 * @data: data relevant (environ)
 */
void change_to_home_dir(shell_data_t *data)
{
	char *p_pwd, *home;
	char pwd[PATH_MAX];

	getcwd(pwd, sizeof(pwd));
	p_pwd = _strdup(pwd);

	home = get_env("HOME", data->_env);

	if (home == NULL)
	{
		set_env("OLDPWD", p_pwd, data);
		free(p_pwd);
		return;
	}

	if (chdir(home) == -1)
	{
		get_error_code(data, 2);
		free(p_pwd);
		return;
	}

	set_env("OLDPWD", p_pwd, data);
	set_env("PWD", home, data);
	free(p_pwd);
	data->status = 0;
}

/**
 * change_directory_shell - this function changes current directory
 * @data: data relevant
 * Return: 1 on success
 */
int change_directory_shell(shell_data_t *data)
{
	char *dir;
	int ishome, ishome2, isddash;

	dir = data->args[1];

	if (dir != NULL)
	{
		ishome = _strcmp("$HOME", dir);
		ishome2 = _strcmp("~", dir);
		isddash = _strcmp("--", dir);
	}

	if (dir == NULL || !ishome || !ishome2 || !isddash)
	{
		change_to_home_dir(data);
		return (1);
	}

	if (_strcmp("-", dir) == 0)
	{
		change_to_ols_dir(data);
		return (1);
	}

	if (_strcmp(".", dir) == 0 || _strcmp("..", dir) == 0)
	{
		change_to_dot_dir(data);
		return (1);
	}

	change_to_dir(data);

	return (1);
}
