/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 02:40:13 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/24 17:16:58 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

extern volatile sig_atomic_t	g_signal;

static t_shcontext	init_context(t_astnode *root, t_node *envl);
static void			visit(t_astnode *node, t_node *envl,
						t_shcontext *mshcontext);
static void			find_rightmost_word(t_astnode *root, t_astnode **to_set);
void				restore_iodes(t_shcontext *mshcontext, bool clear);

int	interpret(t_astnode *root, t_node *envl)
{
	t_shcontext	mshcontext;

	if (!root)
		return (EXIT_FAILURE);
	mshcontext = init_context(root, envl);
	mshcontext.exit_status = *(int *)envl->content;
	signal(SIGINT, SIG_IGN);
	find_rightmost_word(root, &mshcontext.rightmost_word);
	visit_prematurely(root, &mshcontext);
	if (mshcontext.terminate)
		return (*(int *)envl->content = 130);
	visit(root, envl, &mshcontext);
	perform_wait_and_fetch_wstatus(&mshcontext);
	restore_iodes(&mshcontext, true);
	return (determine_exit_code(&mshcontext));
}

static void	visit(t_astnode *node, t_node *envl, t_shcontext *mshcontext)
{
	if (!node)
		return ;
	prepare_pipenode(node, mshcontext);
	handle_lredir(node, mshcontext);
	handle_rredir(node, mshcontext);
	visit(node->left, envl, mshcontext);
	handle_pipe(node, mshcontext);
	visit(node->right, envl, mshcontext);
	handle_word(node, envl, mshcontext);
}

static void	find_rightmost_word(t_astnode *root, t_astnode **to_set)
{
	if (!root)
		return ;
	if (root->type == TK_WORD)
		*to_set = root;
	find_rightmost_word(root->left, to_set);
	find_rightmost_word(root->right, to_set);
}

static t_shcontext	init_context(t_astnode *root, t_node *envl)
{
	return ((t_shcontext)
		{
			.envl = envl,
			.terminate = false,
			.permissions_clear = true,
			.root = root,
			.last_command = NULL,
			.rightmost_word = NULL,
			.wstatus = -1,
			.exit_status = 0,
			.stds = {dup(STDIN_FILENO), dup(STDOUT_FILENO)}
		});
}

void	restore_iodes(t_shcontext *mshcontext, bool clear)
{
	dup2(mshcontext->stds[0], STDIN_FILENO);
	close(mshcontext->stds[0]);
	dup2(mshcontext->stds[1], STDOUT_FILENO);
	close(mshcontext->stds[1]);
	if (!clear)
	{
		mshcontext->stds[0] = dup(STDIN_FILENO);
		mshcontext->stds[1] = dup(STDOUT_FILENO);
	}
}
