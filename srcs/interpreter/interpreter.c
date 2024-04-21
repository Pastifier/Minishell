/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 02:40:13 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/19 13:43:03 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"

extern int	g_signal;

static void	visit(t_astnode *node, t_node *envl, int *fetch);

void	interpret(t_astnode *root, t_node *envl)
{
	// int	std_in;
	int	fetch;

	// std_in = dup(STDIN_FILENO)
	visit(root, envl, &fetch);
	// wait for children (if any)
	// dup2(fetch, std_in)
}

static void	visit(t_astnode *node, t_node *envl, int *fetch)
{
	if (!node)
		return ;
	// Pre-order stuff
	prepare_pipenode(node);
	prepare_rredir(node);

	// Traversal
	visit(node->left, envl, fetch);
	visit(node->right, envl, fetch);

	// Post-order stuff
	// handle_pipe(node);
	handle_word(node, envl);
}
