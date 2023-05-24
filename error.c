#include "shell.h"

/**
 * check_rep - this function checks for repeated characters
 * @input: input string
 * @i: index
 * Return: repetitions of characters
 */
int check_rep(char *input, int i)
{
	if (*(input - 1) == *input)
	{
		return (check_rep(input - 1, i + 1));
	}
	else
	{
		return (i);
	}
}

/**
 * check_err - this function finds syntax errors
 * @input: input string
 * @i: index
 * @last: last character read
 * Return: index of error. 0 when there are no errors
 */
int check_err(char *input, int i, char last)
{
	int count = 0;

	if (*input == '\0')
		return (0);

	if (*input == ' ' || *input == '\t')
		return (check_err(input + 1, i + 1, last));

	if (*input == ';')
		if (last == '|' || last == '&' || last == ';')
			return (i);

	if (*input == '|')
	{
		if (last == ';' || last == '&')
			return (i);

		if (last == '|')
		{
			count = check_rep(input, 0);
			if (count == 0 || count > 1)
				return (i);
		}
	}
	if (*input == '&')
	{
		if (last == ';' || last == '|')
			return (i);

		if (last == '&')
		{
			count = check_rep(input, 0);
			if (count == 0 || count > 1)
				return (i);
		}
	}
	return (check_err(input + 1, i + 1, *input));
}
/**
 * check_first_char - this function finds the index of the first char
 * @input: input string
 * @i: index
 * Return: 1 if there is an error. 0 in other case.
 */
int check_first_char(char *input, int *i)
{
	*i = 0;

	while (input[*i])
	{
		if (input[*i] == ' ' || input[*i] == '\t')
		{
			++*i;
			continue;
		}
		if (input[*i] == ';' || input[*i] == '|' || input[*i] == '&')
		{
			return (-1);
		}
		break;
	}
	return (0);
}

/**
 * print_syntax_err - this function prints when a syntax error is found
 * @data: data structure
 * @input: input string
 * @i: index of the error
 * @bool: to control msg error
 */
void print_syntax_err(shell_data_t *data, char *input, int i, int bool)
{
	char *msg, *msg2, *msg3, *error, *counter;
	int length;

	if (input[i] == ';')
	{
		if (bool == 0)
			msg = (input[i + 1] == ';' ? ";;" : ";");
		else
			msg = (input[i - 1] == ';' ? ";;" : ";");
	}
	if (input[i] == '|')
		msg = (input[i + 1] == '|' ? "||" : "|");

	if (input[i] == '&')
		msg = (input[i + 1] == '&' ? "&&" : "&");

	msg2 = ": Syntax error: \"";
	msg3 = "\" unexpected\n";
	counter = int_to_string(data->counter);
	length = _strlen(data->av[0]) + _strlen(counter) + _strlen(msg) +
		_strlen(msg2) + _strlen(msg3) + 3;

	error = malloc(sizeof(char) * (length));
	if (error == NULL)
	{
		free(counter);
		return;
	}
	_strcpy(error, data->av[0]);
	_strcat(error, ": ");
	_strcat(error, counter);
	_strcat(error, msg2);
	_strcat(error, msg);
	_strcat(error, msg3);
	_strcat(error, "\0");
	error[length - 1] = '\0';

	write(STDERR_FILENO, error, length - 1);
	free(error);
	free(counter);
}

/**
 * check_syntax_err - this function finds and prints a syntax error
 * @data: data structure
 * @input: input string
 * Return: 1 if there is an error. 0 in other case
 */
int check_syntax_err(shell_data_t *data, char *input)
{
	int begin = 0, f_char = 0, error_pos = 0;

	f_char = check_first_char(input, &begin);
	if (f_char == -1)
	{
		print_syntax_err(data, input, begin, 0);
		return (1);
	}
	error_pos = check_err(input + begin, 0, input[begin]);
	if (error_pos != 0)
	{
		print_syntax_err(data, input, begin + error_pos, 1);
		return (1);
	}
	return (0);
}
