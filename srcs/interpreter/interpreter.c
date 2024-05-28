/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 02:40:13 by ebinjama          #+#    #+#             */
/*   Updated: 2024/05/28 03:06:53 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"
#include <stdio.h>

extern int	g_signal;

static void	visit(t_astnode *node, t_node *envl, int *stds);
static void	find_rightmost_word(t_astnode *root, t_astnode **to_set);

int	interpret(t_astnode *root, t_node *envl)
{
	int			fetch;
	int			wstatus;
	int			stds[2];
	int			exit_status;
	t_astnode	*rightmost_word;

	if (!root)
		return (EXIT_FAILURE);
	rightmost_word = NULL;
	stds[0] = dup(STDIN_FILENO);
	stds[1] = dup(STDOUT_FILENO);
	find_rightmost_word(root, &rightmost_word);
	visit(root, envl, stds);
	// wait for children (if any)
	fetch = 1;
	exit_status = 0;
	while (fetch > 0)
	{
		fetch = wait(&wstatus);
		if (fetch == rightmost_word->data.command.pid)
			exit_status = wstatus;
	}
	dup2(stds[0], STDIN_FILENO);
	dup2(stds[1], STDOUT_FILENO);
	close(stds[0]);
	close(stds[1]);
	return (WEXITSTATUS(exit_status));
}

static void	visit(t_astnode *node, t_node *envl, int *stds)
{
	if (!node)
		return;
	// Pre-order stuff
	prepare_pipenode(node);
	handle_lredir(node);
	handle_rredir(node);

	// Traversal
	visit(node->left, envl, stds);
	handle_pipe(node, stds);
	visit(node->right, envl, stds);

	// Post-order stuff
	handle_word(node, envl);
}

void find_rightmost_word(t_astnode *root, t_astnode **to_set)
{
	if (!root)
		return;
	if (root->type == TK_WORD)
		*to_set = root;
	find_rightmost_word(root->left, to_set);
	find_rightmost_word(root->right, to_set);
}
