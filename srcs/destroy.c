/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 17:10:38 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/25 17:24:49 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void	destroy_str_arr(char **str_arr)
{
	int	i;

	if (!str_arr || !*str_arr)
		return ;
	i = 0;
	while (str_arr[i])
	{
		free(str_arr[i]);
		i++;
	}
	free(str_arr);
}

void	destroy_tokens(t_token **tokens)
{
	t_token		*temp;
	t_token		**original;

	if (!tokens || !*tokens)
		return ;
	original = &(*tokens);
	while (*tokens)
	{
		temp = *tokens;
		*tokens = (*tokens)->next;
		free(temp->value);
		free(temp);
	}
	*original = NULL;
}

void	destroy_ast(t_astnode *node)
{
	if (!node)
		return ;
	destroy_ast(node->left);
	destroy_ast(node->right);
	if (node->type == TK_WORD)
	{
		free(node->data.command.cmd);
		list_destroy(&node->data.command.args);
		free(node);
	}
	else if (node->type == TK_LREDIR || node->type == TK_RREDIR)
	{
		free(node->data.redirection.filename);
		free(node);
	}
	else
		free(node);
}

void	destroy_mini_shell(t_token **token, t_astnode **node, int exit_status)
{
	(void)token;
	destroy_ast(*node);
	show_error(exit_status);
	*node = NULL;
}
