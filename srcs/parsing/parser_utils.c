/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 17:01:01 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/19 17:07:32 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

char	**get_command_args(t_token **token)
{
	char	**args;
	t_token	*temp;
	int		i;

	i = 0;
	temp = *token;
	while (temp && temp->token_type == TK_WORD)
	{
		i++;
		temp = temp->next;
	}
	args = ft_calloc(i + 1, sizeof(char *));
	if (!args)
		return (NULL);
	temp = *token;
	i = 0;
	while (temp && temp->token_type == TK_WORD)
	{
		args[i] = ft_strdup(temp->value);
		if (!args[i])
			return (destroy_str_arr(args), NULL);
		i++;
		temp = temp->next;
	}
	return (args[i] = NULL, (*token)->next = temp, args);
}

void	add_redir_node(t_astnode **node, t_astnode *new_node)
{
	t_astnode	*iter;

	if (!(*node))
	{
		(*node) = new_node;
		new_node->parent = NULL;
	}
	else
	{
		iter = *node;
		while (iter && iter->right)
			iter = iter->right;
		new_node->parent = iter;
		iter->right = new_node;
	}
}

void	set_redir_type(t_astnode *new_node, t_token_type type)
{
	if (type == TK_LAPPEND || type == TK_RAPPEND)
		new_node->data.redirection.mode = O_APPEND;
	else
		new_node->data.redirection.mode = 0;
	if (type == TK_LREDIR || type == TK_LAPPEND)
		new_node->type = TK_LREDIR;
	else
		new_node->type = TK_RREDIR;
}

void	remove_token(t_token **token_list)
{
	t_token	*iter;

	iter = *token_list;
	if (!iter->next && !iter->prev)
		*token_list = NULL;
	else if (!iter->prev)
	{
		*token_list = iter->next;
		(*token_list)->prev = iter->prev;
	}
	else if (!iter->next)
	{
		iter->prev->next = NULL;
		(*token_list) = NULL;
	}
	else
	{
		iter->prev->next = iter->next;
		iter->next->prev = iter->prev;
		(*token_list) = iter->next;
	}
	free(iter->value);
	free(iter);
	iter = NULL;
}
