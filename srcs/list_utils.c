#include "minishell.h"

t_node	*node_create(void *content)
{
	t_node	*self;

	self = malloc(sizeof(t_node));
	if (!self)
		return (NULL);
	self->prev = NULL;
	self->next = NULL;
	self->content = content;
	return (self);
}

void	node_destroy(t_node *node)
{
	t_node	*prev;
	t_node	*next;

	prev = node->prev;
	next = node->next;
	next->prev = prev;
	prev->next = next;
	node->prev = NULL;
	node->next = NULL;
	free(node->content);
	free(node);
}

void	list_append(t_node **head, t_node *to_append)
{
	t_node	*iter;

	if (*head)
	{
		iter = *head;
		while (iter->next)
		{
			to_append->prev = iter;
			iter = iter->next;
		}
		iter->next = to_append;
		to_append->next = NULL;
	}
	else
		*head = to_append;
}

void	list_destroy(t_node **head)
{
	t_node	*temp;
	t_node	*iter;

	if (!head)
		return ;
	iter = *head;
	while (iter)
	{
		temp = iter->next;
		node_destroy(iter);
		iter = temp;
	}
	*head = NULL;
}

bool	str_arr_dup_to_list(char **strarr, t_node **head)
{
	t_node	*to_append;
	void	*content;

	while (strarr)
	{
		content = ft_strdup(*strarr);
		if (!content)
			return (list_destroy(head), false);
		to_append = node_create(content);
		if (!to_append)
			return (free(content), list_destroy(head), false);
		list_append(head, node_create(ft_strdup(*strarr)));
		++strarr;
	}
}
