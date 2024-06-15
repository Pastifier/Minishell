/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 22:21:40 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/15 09:04:10 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*node_create(void *content)
{
	t_node	*self;

	self = malloc(sizeof(t_node));
	if (!self)
		return (NULL);
	self->prev = NULL;
	self->next = NULL;
	self->visible = true;
	self->content = ft_strdup(content);
	if (!self->content)
	{
		free(self);
		return (NULL);
	}
	return (self);
}

void	node_destroy(t_node *node)
{
	t_node	*prev;
	t_node	*next;

	if (!node)
		return ;
	prev = node->prev;
	next = node->next;
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;
	node->prev = NULL;
	node->next = NULL;
	free(node->content);
	node->content = NULL;
	free(node);
}

void	list_append(t_node **head, t_node *to_append)
{
    t_node	*iter;

    if (!head || !to_append)
        return ;
    if (*head)
    {
        iter = *head;
        while (iter->next)
            iter = iter->next;
        to_append->prev = iter;
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
		temp = iter;
		iter = iter->next;
		free(temp->content);
		free(temp);
	}
	*head = NULL;
}

bool	str_arr_dup_to_list(char **strarr, t_node **head)
{
	t_node	*to_append;

	while (strarr && *strarr)
	{
		to_append = node_create(*strarr);
		if (!to_append)
			return (list_destroy(head), false);
		list_append(head, to_append);
		++strarr;
	}
	return (true);
}
