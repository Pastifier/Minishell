/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 02:40:13 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/16 03:16:01 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"

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
	// Pre-order stuff
	prepare_pipenode(node);

	// Traversal
	visit(node->left, envl);
	visit(node->right, envl);

	// Post-order stuff
	handle_pipe(node, envl);
	handle_word(node, envl);
}
