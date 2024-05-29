/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 02:40:13 by ebinjama          #+#    #+#             */
/*   Updated: 2024/05/29 14:32:06 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

extern int	g_signal;

static t_shcontext	init_context(t_astnode *root);
static void			visit(t_astnode *node, t_node *envl,
	t_shcontext *mshcontext);
static void			find_rightmost_word(t_astnode *root, t_astnode **to_set);
static void			restore_iodes(t_shcontext *mshcontext);

int	interpret(t_astnode *root, t_node *envl)
{
	int			fetch;
	t_shcontext	mshcontext;

	if (!root)
		return (EXIT_FAILURE);
	init_context(root);
	mshcontext.stds[0] = dup(STDIN_FILENO);
	mshcontext.stds[1] = dup(STDOUT_FILENO);
	mshcontext.envl = envl;
	find_rightmost_word(root, &mshcontext.rightmost_word);
	visit(root, envl, &mshcontext);
	fetch = 1;
	while (fetch > 0)
	{
		fetch = wait(&mshcontext.wstatus);
		if (fetch == mshcontext.rightmost_word->data.command.pid)
			mshcontext.exit_status = mshcontext.wstatus;
	}
	restore_iodes(&mshcontext);
	return (WEXITSTATUS(mshcontext.exit_status));
}

static void	visit(t_astnode *node, t_node *envl, t_shcontext *mshcontext)
{
	if (!node)
		return;
	// Pre-order stuff
	prepare_pipenode(node, mshcontext);
	handle_lredir(node, mshcontext);
	handle_rredir(node, mshcontext);

	// Traversal
	visit(node->left, envl, mshcontext);
	handle_pipe(node, mshcontext);
	visit(node->right, envl, mshcontext);

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

static t_shcontext	init_context(t_astnode *root)
{
	return ((t_shcontext)
	{
		.terminate = false,
		.permissions_clear = true,
		.root = root,
		.last_command = NULL,
		.rightmost_word = NULL,
		.exit_status = 0,
	});
}

static void	restore_iodes(t_shcontext *mshcontext)
{
	dup2(mshcontext->stds[0], STDIN_FILENO);
	dup2(mshcontext->stds[1], STDOUT_FILENO);
	close(mshcontext->stds[0]);
	close(mshcontext->stds[1]);
	mshcontext->stds[0] = dup(STDIN_FILENO);
	mshcontext->stds[1] = dup(STDOUT_FILENO);
}