#include "shell.h"

/**
 * _strdup - this function duplicates a str in the heap memory.
 * @s: string parameter to be passed
 * Return: returns the duplicated string
 */
char *_strdup(const char *s)
{
	size_t len = _strlen(s) + 1;
	char *copy = malloc(sizeof(char) * len);

	if (copy != NULL)
		_memcpy(copy, s, len);

	return (copy);
}

/**
 * _strlen - this function returns the lenght of a string.
 * @s: string parameter to be passed
 * Return: return the length of the string
 */
int _strlen(const char *s)
{
	int len = 0;

	while (*s != '\0')
	{
		len++;
		s++;
	}
	return (len);
}

/**
 * cmp_chars - this function compares characters of strings
 * @str: input string
 * @delim: delimiter
 * Return: 1 if strings are equal and 0 if otherwise
 */
int cmp_chars(char str[], const char *delim)
{
	unsigned int i, j, k = 0;

	for (i = 0; str[i]; i++)
	{
		for (j = 0; delim[j]; j++)
		{
			if (str[i] == delim[j])
			{
				k++;
				break;
			}
		}
	}
	return ((i == k) ? 1 : 0);
}

/**
 * _str_del - splits a string by some delimiter.
 * @str: input string.
 * @del: delimiter.
 * Return: string splited.
 */
char *_str_del(char str[], const char *del)
{
	char *current_char;
	unsigned int i, token_founded;
	static char *token_start, *token_end;

	if (str != NULL)
	{
		if (cmp_chars(str, del))
			return (NULL);
		token_start = str; /* Storing first address */
		i = _strlen(str);
		token_end = &str[i]; /* Storing last address */
	}
	current_char = token_start;
	if (current_char == token_end) /* Reaching the end */
		return (NULL);

	for (token_founded = 0; *token_start; token_start++)
	{
		/*  finding the next token */
		if (token_start != current_char)
			if (*token_start && *(token_start - 1) == '\0')
				break;
		/* Replacing delimiter for nullchar */
		for (i = 0; del[i]; i++)
		{
			if (*token_start == del[i])
			{
				*token_start = '\0';
				if (token_start == current_char)
					current_char++;
				break;
			}
		}
		if (token_founded == 0 && *token_start) /* Str != Delimiter */
			token_founded = 1;
	}
	if (token_founded == 0)
		return (NULL);
	return (current_char);
}

/**
 * _isdigit - this function checks if an input string paramater is a digit
 * @s: input string
 * Return: returns 1 if string is a number and 0 if otherwise
 */
int _isdigit(const char *s)
{
	unsigned int i;

	for (i = 0; s[i]; i++)
	{
		if (s[i] < 48 || s[i] > 57)
		{
			return (0);
		}
	}
	return (1);
}
