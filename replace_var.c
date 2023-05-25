#include "shell.h"

/**
 * check_for_environment_variables - function checks
 *					if the typed variable is an env variable
 * @h: pointer to head of linked list
 * @in: pointer to input string
 * @data: data structure
 */
void check_for_environment_variables(
		replacement_variable_t **h, char *in, shell_data_t *data)
{
	int row, chr, j, lval;
	char **_envr = data->_env;

	for (row = 0; _envr[row]; row++)
	{
		for (j = 1, chr = 0; _envr[row][chr]; chr++)
		{
			if (_envr[row][chr] == '=')
			{
				lval = _strlen(_envr[row] + chr + 1);
				add_replacement_variable_node(
						h, j, _envr[row] + chr + 1, lval);
				return;
			}

			if (in[j] == _envr[row][chr])
				j++;
			else
				break;
		}
	}
	for (j = 0; in[j]; j++)
	{
		if (in[j] == ' ' || in[j] == '\t' || in[j] == ';' || in[j] == '\n')
			break;
	}
	add_replacement_variable_node(h, j, NULL, 0);
}
/**
 * replace_variables - this function checks if the typed variable is $$ or $?
 * @h: head of the linked list
 * @in: input string
 * @st: last status of the Shell
 * @data: data structure
 * Return: returns an integer
 */
int replace_variables(
		replacement_variable_t **h, char *in, char *st, shell_data_t *data)
{
	int i, lst = _strlen(st), lpd = _strlen(data->pid);

	for (i = 0; in[i]; i++)
	{
		if (in[i] == '$')
		{
			if (in[i + 1] == '?')
				add_replacement_variable_node(h, 2, st, lst), i++;
			else if (in[i + 1] == '$')
				add_replacement_variable_node(h, 2, data->pid, lpd), i++;
			else if (in[i + 1] == '\n')
				add_replacement_variable_node(h, 0, NULL, 0);
			else if (in[i + 1] == '\0')
				add_replacement_variable_node(h, 0, NULL, 0);
			else if (in[i + 1] == ' ')
				add_replacement_variable_node(h, 0, NULL, 0);
			else if (in[i + 1] == '\t')
				add_replacement_variable_node(h, 0, NULL, 0);
			else if (in[i + 1] == ';')
				add_replacement_variable_node(h, 0, NULL, 0);
			else
				check_for_environment_variables(h, in + i, data);
		}
	}
	return (i);
}

/**
 * get_replaced_input - replaces string into variables
 * @head: head of the linked list
 * @input: input string
 * @new_input: new input string (replaced)
 * @nlen: new length
 * Return: replaced string
 */
char *get_replaced_input(
		replacement_variable_t **head, char *input, char *new_input, int nlen)
{
	int i, j, k;
	replacement_variable_t *index = *head;

	for (j = i = 0; i < nlen; i++)
	{
		if (input[j] == '$')
		{
			if (!(index->len_var) && !(index->len_val))
			{
				new_input[i] = input[j];
				j++;
			}
			else if (index->len_var && !(index->len_val))
			{
				for (k = 0; k < index->len_var; k++)
					j++;
				i--;
			}
			else
			{
				for (k = 0; k < index->len_val; k++)
				{
					new_input[i] = index->val[k];
					i++;
				}
				j += (index->len_var);
				i--;
			}
			index = index->next;
		}
		else
		{
			new_input[i] = input[j];
			j++;
		}
	}
	return (new_input);
}

/**
 * *replace_variable - function calls functions to replace string into vars
 * @input: input string
 * @datash: data structure
 * Return: replaced string
 */
char *replace_variable(char *input, shell_data_t *datash)
{
	replacement_variable_t *head, *index;
	char *status, *new_input;
	int olen, nlen;

	status = int_to_string(datash->status);
	head = NULL;

	olen = replace_variables(&head, input, status, datash);

	if (head == NULL)
	{
		free(status);
		return (input);
	}

	index = head;
	nlen = 0;

	while (index != NULL)
	{
		nlen += (index->len_val - index->len_var);
		index = index->next;
	}

	nlen += olen;

	new_input = malloc(sizeof(char) * (nlen + 1));
	new_input[nlen] = '\0';

	new_input = get_replaced_input(&head, input, new_input, nlen);

	free(input);
	free(status);
	free_replacement_variable_list(&head);

	return (new_input);
}
