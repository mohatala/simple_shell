#include "shell.h"

/**
 * read_input_line_from_buffer - assigns the line var for get_line
 * @lineptr: Buffer that store the input str
 * @buffer: str that is been called to line
 * @n: size of line
 * @j: size of buffer
 */
void read_input_line_from_buffer(
		char **lineptr, size_t *n, char *buffer, size_t j)
{

	if (*lineptr == NULL)
	{
		if  (j > BUFFER_SIZE)
			*n = j;

		else
			*n = BUFFER_SIZE;
		*lineptr = buffer;
	}
	else if (*n < j)
	{
		if (j > BUFFER_SIZE)
			*n = j;
		else
			*n = BUFFER_SIZE;
		*lineptr = buffer;
	}
	else
	{
		_strcpy(*lineptr, buffer);
		free(buffer);
	}
}
/**
 * get_input_line - Read inpt from stream
 * @lineptr: buffer that stores the input
 * @n: size of lineptr
 * @stream: stream to read from
 * Return: The number of bytes
 */
ssize_t get_input_line(char **lineptr, size_t *n, FILE *stream)
{
	int i;
	static ssize_t input;
	ssize_t retval;
	char *buffer;
	char t = 'z';

	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * BUFFER_SIZE);
	if (buffer == 0)
		return (-1);
	while (t != '\n')
	{
		i = read(STDIN_FILENO, &t, 1);
		if (i == -1 || (i == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (i == 0 && input != 0)
		{
			input++;
			break;
		}
		if (input >= BUFFER_SIZE)
			buffer = _realloc(buffer, input, input + 1);
		buffer[input] = t;
		input++;
	}
	buffer[input] = '\0';
	read_input_line_from_buffer(lineptr, n, buffer, input);
	retval = input;
	if (i != 0)
		input = 0;
	return (retval);
}

/**
 * read_input_line - reads the input string.
 * @i_eof: return value of getline function
 * Return: input string
 */
char *read_input_line(int *i_eof)
{
	char *input = NULL;
	size_t bufsize = 0;

	*i_eof = getline(&input, &bufsize, stdin);

	return (input);
}

/**
 * execute_input_line - finds builtins and commands
 * @datash: data relevant (args)
 * Return: 1 on success.
 */
int execute_input_line(shell_data_t *datash)
{
	int (*builtin)(shell_data_t *datash);

	if (datash->args[0] == NULL)
		return (1);

	builtin = get_builtin_function(datash->args[0]);

	if (builtin != NULL)
		return (builtin(datash));

	return (execute_command(datash));
}
