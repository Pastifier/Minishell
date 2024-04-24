/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 02:40:13 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/24 02:38:22 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"
#include <stdio.h>

extern int	g_signal;

static void	visit(t_astnode *node, t_node *envl, int *fetch);
static void	find_rightmost_word(t_astnode *root, t_astnode **to_set);

int	interpret(t_astnode *root, t_node *envl)
{
	// int	std_in;
	int			fetch;
	int			wstatus;
	int			exit_status;
	t_astnode	*rightmost_word;

	// std_in = dup(STDIN_FILENO)
	if (!root)
		return (EXIT_FAILURE);
	rightmost_word = NULL;
	find_rightmost_word(root, &rightmost_word);
	visit(root, envl, &fetch);
	// wait for children (if any)
	fetch = 1;
	exit_status = 0;
	while (fetch > 0)
	{
		fetch = wait(&wstatus);
		if (fetch == rightmost_word->data.command.pid)
			exit_status = wstatus;
	}
	return (WEXITSTATUS(exit_status));
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

void	find_rightmost_word(t_astnode *root, t_astnode **to_set)
{
	if (!root)
		return ;
	find_rightmost_word(root->right, to_set);
	if (root->type == TK_WORD && !root->left && !root->right && !(*to_set))
	{
		*to_set = root;
		return ;
	}
	find_rightmost_word(root->left, to_set);
}