/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:59:25 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/26 15:11:31 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <stdio.h>

int	parse(t_token **tokens_iter, t_astnode **node, t_node **envl)
{
	int	ret;

	ret = 0;
	if ((*tokens_iter)->token_type == TK_WORD)
		ret = parse_word(tokens_iter, node);
	else if ((*tokens_iter)->token_type == TK_PIPE)
		ret = parse_pipe(tokens_iter, node);
	else if ((*tokens_iter)->token_type == TK_LREDIR
		|| (*tokens_iter)->token_type == TK_LAPPEND)
		ret = parse_redir(tokens_iter, node);
	else if ((*tokens_iter)->token_type == TK_RREDIR
		|| (*tokens_iter)->token_type == TK_RAPPEND)
		ret = parse_redir(tokens_iter, node);
	if (ret)
		return (ret);
	if (*tokens_iter && (*tokens_iter)->next != NULL)
	{
		*tokens_iter = (*tokens_iter)->next;
		return (parse(tokens_iter, node, envl));
	}
	return (0);
}

int	parse_word(t_token **token_list, t_astnode **node)
{
	if (!(*node))
		return (initializ_new_ast_node(token_list, node));
	else if ((*node) && (*node)->type == TK_PIPE)
		return (set_word_in_pipe(token_list, node));
	else if ((*node) && (*node)->type == TK_RREDIR)
		return (set_word_in_rredir(token_list, node));
	else if ((*node) && (*node)->type == TK_WORD)
		return (set_word_in_word(token_list, node));
	else if ((*node) && (*node)->type == TK_LREDIR)
		return (set_word_in_lredir(token_list, node));
	return (0);
}

int	parse_pipe(t_token **token_list, t_astnode **node)
{
	t_astnode	*new_node;

	if ((*token_list)->prev && !(*token_list)->next)
		return (4);
	if (!(*token_list)->prev || ((*node) && ((*node)->type == TK_AND
				|| (*node)->type == TK_OR))
		|| (*token_list)->next->token_type == TK_PIPE)
		return (show_syntax_error((*token_list)->value), 6);
	new_node = (t_astnode *)malloc(sizeof(t_astnode));
	if (new_node == NULL)
		return (1);
	new_node->type = TK_PIPE;
	if (!(*node))
		new_node->left = NULL;
	else
	{
		new_node->left = *node;
		(*node)->parent = new_node;
	}
	new_node->left = *node;
	new_node->parent = NULL;
	*node = new_node;
	new_node->right = NULL;
	return (0);
}

int	parse_redir(t_token **token_list, t_astnode **node)
{
	t_astnode	*new_node;

	if (!(*token_list)->next || (*token_list)->next->token_type != TK_WORD)
		return (show_syntax_error("newline"), 6);
	new_node = (t_astnode *)malloc(sizeof(t_astnode));
	if (new_node == NULL)
		return (1);
	set_redir_type(new_node, (*token_list)->token_type);
	add_redir_node(node, new_node);
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->data.redirection.filename = ft_strdup((*token_list)->next->value);
	*token_list = (*token_list)->next;
	return (0);
}

int	parse_export(char *var_name)
{
	int	i;

	i = 0;
	if (ft_isalpha(var_name[i]) || var_name[i] == '_')
	{
		i++;
		while (var_name[i] && (ft_isalnum(var_name[i]) || var_name[i] == '_'))
			i++;
		if (!var_name[i])
			return (0);
	}
	return (1);
}
