#include "minishell.h"

t_node	*node_create(void *content)
{
	t_node	*self;

	self = malloc(sizeof(t_node));
	self->prev = NULL;
	self->next = NULL;
	self->content = content;
	return (self);
}

void	node_destroy(t_node *node)
{
	free(node->content);
	node->content = NULL;
	free(node);
}

void	list_append(t_node *head, t_node *to_append)
{
	t_node	*iter;

	if (head)
	{
		iter = head;
		while (iter)
			iter = iter->next;
		iter = to_append;
	}
	else
		head = to_append;
}
