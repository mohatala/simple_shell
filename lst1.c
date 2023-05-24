#include "shell.h"

/**
 * add_separator_node_end - this function adds a separator found at the end of
 *				a sep_list_t.
 * @head: pointer to pointer to head node of linked list
 * @sep: sep found (; | &).
 * Return: returns a pointer to the head node
 */
sep_list_t *add_separator_node_end(sep_list_t **head, char sep)
{
	sep_list_t *new_node, *temp_node = *head;

	new_node = malloc(sizeof(sep_list_t));
	if (new_node == NULL)
		return (NULL);

	new_node->sep = sep;
	new_node->next = NULL;

	if (temp_node == NULL)
		*head = new_node;
	else
	{
		while (temp_node->next != NULL)
		{
			temp_node = temp_node->next;
		}
		temp_node->next = new_node;
	}
	return (*head);
}

/**
 * free_sep_list - this function frees a sep_list_t
 * @head: pointer to pointer to head node of linked list
 */
void free_sep_list(sep_list_t **head)
{
	sep_list_t *temp_node = NULL, *curr_node = *head;

	if (head == NULL || *head == NULL)
		return;

	for (; curr_node != NULL; curr_node = temp_node)
	{
		temp_node = curr_node->next;
		free(curr_node);
	}
	*head = NULL;
}

/**
 * add_command - this function adds a command line at the end of a
 *				cmd_lst_t.
 * @head: pointer to pointer to head node of linked list
 * @line: command line.
 * Return: returns a pointer to the head node
 */
cmd_lst_t *add_command(cmd_lst_t **head, char *line)
{
	cmd_lst_t *new_node, *temp_node = *head;

	new_node = malloc(sizeof(cmd_lst_t));
	if (new_node == NULL)
		return (NULL);

	new_node->line = line;
	new_node->next = NULL;

	if (temp_node == NULL)
		*head = new_node;
	else
	{
		while (temp_node->next != NULL)
		{
			temp_node = temp_node->next;
		}
		temp_node->next = new_node;
	}
	return (*head);
}

/**
 * free_command_list - this function frees a cmd_lst_t
 * @head: pointer to pointer to head node of linked list
 */
void free_command_list(cmd_lst_t **head)
{
	cmd_lst_t *temp_node = NULL, *current_node = *head;

	if (head == NULL || *head == NULL)
		return;

	for (; current_node != NULL; current_node = temp_node)
	{
		temp_node = current_node->next;
		free(current_node);
	}
	*head = NULL;
}
