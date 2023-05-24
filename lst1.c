#include "shell.h"

/**
 * add_separator_node_end - this function adds a separator found at the end of
 *				a separator_list_t.
 * @head: pointer to pointer to head node of linked list
 * @sep: separator found (; | &).
 * Return: returns a pointer to the head node
 */
separator_list_t *add_separator_node_end(separator_list_t **head, char sep)
{
	separator_list_t *new_node, *temp_node = *head;

	new_node = malloc(sizeof(separator_list_t));
	if (new_node == NULL)
		return (NULL);

	new_node->separator = sep;
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
 * free_separator_list - this function frees a separator_list_t
 * @head: pointer to pointer to head node of linked list
 */
void free_separator_list(separator_list_t **head)
{
	separator_list_t *temp_node = NULL, *curr_node = *head;

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
 * add_command_node_end - this function adds a command line at the end of a
 *				command_list_t.
 * @head: pointer to pointer to head node of linked list
 * @line: command line.
 * Return: returns a pointer to the head node
 */
command_list_t *add_command_node_end(command_list_t **head, char *line)
{
	command_list_t *new_node, *temp_node = *head;

	new_node = malloc(sizeof(command_list_t));
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
 * free_command_list - this function frees a command_list_t
 * @head: pointer to pointer to head node of linked list
 */
void free_command_list(command_list_t **head)
{
	command_list_t *temp_node = NULL, *curr_node = *head;

	if (head == NULL || *head == NULL)
		return;

	for (; curr_node != NULL; curr_node = temp_node)
	{
		temp_node = curr_node->next;
		free(curr_node);
	}
	*head = NULL;
}
