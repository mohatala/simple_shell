#include "shell.h"

/**
 * _memcpy - this function copies information between void pointers.
 * @newptr: destination pointer.
 * @ptr: source pointer.
 * @size: size of the new pointer.
 */
void _memcpy(void *newptr, const void *ptr, unsigned int size)
{
	unsigned int i;
	char *dest = (char *) newptr;
	const char *src = (const char *) ptr;

	for (i = 0; i < size; i++)
		dest[i] = src[i];
}

/**
 * _realloc - this function reallocates a memory block.
 * @ptr: pointer to the memory previously allocated.
 * @old_size: size, in bytes, of the allocated memory for ptr.
 * @new_size: new size, in bytes, of the new memory block.
 * Return: returns a pointer or NULL if function fails
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *newptr;
	unsigned int i;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}

	if (new_size == old_size)
		return (ptr);

	newptr = malloc(new_size);
	if (newptr == NULL)
		return (NULL);

	if (ptr == NULL)
	{
		return (newptr);
	}
	else
	{
		char *p = ptr, *np = newptr;

		for (i = 0; i < old_size; i++)
			np[i] = p[i];

		free(ptr);
	}
	return (newptr);
}

/**
 * _reallocdp - this function reallocates a memory block of a double pointer.
 * @ptr: double pointer to the memory previously allocated.
 * @old_size: size, in bytes, of the allocated memory of ptr.
 * @new_size: new size, in bytes, of the new memory block.
 * Return: returns a pointer or NULL if function fails
 */
char **_reallocdp(char **ptr, unsigned int old_size, unsigned int new_size)
{
	char **newptr;
	unsigned int i;

	if (new_size == 0)
	{
		/* Free all memory allocated to the old pointer array and return NULL */
		for (i = 0; i < old_size; i++)
			free(ptr[i]);
		free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
	{
		newptr = malloc(sizeof(char *) * new_size);
		if (newptr == NULL)
			return (NULL);
		/* Initialize new pointers to NULL */
		for (i = 0; i < new_size; i++)
			newptr[i] = NULL;
		return (newptr);
	}
	if (new_size == old_size)
		return (ptr);
	/* Allocate new memory for the pointer array */
	newptr = malloc(sizeof(char *) * new_size);
	if (newptr == NULL)
	{
		for (i = 0; i < old_size; i++)
			free(ptr[i]);
		free(ptr);
		return (NULL);
	}
	/* Copy pointers from old pointer array to new pointer array */
	for (i = 0; i < old_size; i++)
		newptr[i] = ptr[i];
	/* Allocate memory or new pointers in new pointer array, if necessary */
	for (i = old_size; i < new_size; i++)
		newptr[i] = NULL;
	free(ptr);
	return (newptr);
}

/**
 * remove_comment - this function removes comments from the input
 * @in: input string
 * Return: input without comments
 */
char *remove_comment(char *in)
{
	int i, j;
	char *out = in;

	for (i = 0, j = 0; in[i] != '\0'; i++)
	{
		if (in[i] == '#' && (i == 0 || in[i - 1] == '\n' ||
					in[i - 1] == ' ' || in[i - 1] == '\t' || in[i - 1] == ';'))
		{
			while (in[i] != '\n' && in[i] != '\0')
				i++;
		}
		else
		{
			out[j++] = in[i];
		}
	}

	out[j] = '\0';

	return (out);
}

/**
 * run_shell_loop - this function runs the shell loop
 * @datash: relevant data (av, input, args)
 */
void run_shell_loop(shell_data_t *datash)
{
	char *input;
	int loop = 1, i_eof;

	while (loop)
	{
		write(STDIN_FILENO, ":p ", 4);
		input = read_input_line(&i_eof);
		if (i_eof != -1)
		{
			input = remove_comment(input);
			if (input == NULL)
				continue;

			if (check_for_syntax_errors(datash, input))
			{
				datash->status = 2;
				continue;
			}
			input = replace_variable(input, datash);
			loop = split_commands_and_operators(datash, input);
			datash->counter++;
		}
		else
		{
			loop = 0;
		}
		free(input);
	}
}
