/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 02:40:13 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/14 08:21:31 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal;

static void	visit(t_astnode *node, t_node *envl);

void	interpret(t_astnode *root, t_node *envl)
{
	visit(root, envl);
}

static void	visit(t_astnode *node, t_node *envl)
{
	if (!node)
		return ;
	// t_node **dupe = list_dup(envl);
	visit(node->left, envl);
	visit(node->right, envl);
	// Post-order stuff
	// if (!node->parent && !node->left && !node->right)
	// 	/*handle_word()*/ ;
	if (node->type == TK_LREDIR)
		{/*handle_rredir()*/}
	else if (node->type == TK_RREDIR)
		{/*handle_rredir()*/}
	else if (node->type == TK_PIPE)
		handle_pipe(node, envl);
	else if (node->type == TK_WORD)
		{/*handle_word()*/}
}
