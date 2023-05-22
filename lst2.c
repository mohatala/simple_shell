#include "shell.h"

/**
 * add_replacement_variable_node - adds a variable node at the end of
 *					a replacement_variable_t list.
 * @head: pointer to pointer to head node of linked list
 * @lvar: variable length
 * @val: pointer to variable value
 * @lval: value length
 * Return: returns a pointer to the head node
 */
replacement_variable_t *add_replacement_variable_node(
		replacement_variable_t **head, int lvar, char *val, int lval)
{
	replacement_variable_t *new_node, *temp_node = *head;

	new_node = malloc(sizeof(replacement_variable_t));
	if (new_node == NULL)
		return (NULL);

	new_node->len_var = lvar;
	new_node->len_val = lval;
	new_node->val = val;
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
 * free_replacement_variable_list - this function frees
 *					a replacement_variable_t list
 * @head: pointer to pointer to head node of linked list
 */
void free_replacement_variable_list(replacement_variable_t **head)
{
	replacement_variable_t *temp_node = NULL, *curr_node = *head;

	if (head == NULL || *head == NULL)
		return;

	for (; curr_node != NULL; curr_node = temp_node)
	{
		temp_node = curr_node->next;
		free(curr_node);
	}
	*head = NULL;
}
