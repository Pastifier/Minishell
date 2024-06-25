/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 01:27:14 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/25 23:44:42 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include "minishell.h"
#include <readline/readline.h>
#include <stdio.h>

static int	store_pipe_in_left_child_if_needed(t_astnode *pipenode,
				t_shcontext *mshcontext);
static int	store_in_closest_left_if_needed(t_astnode *pipenode,
				t_shcontext *mshcontext);

int	prepare_pipenode(t_astnode *pipenode, t_shcontext *mshcontext)
{
	t_astnode	*left_child;

	if (pipenode->type != TK_PIPE)
		return (EXIT_NEEDED);
	left_child = pipenode->left;
	if (left_child && left_child->type == TK_WORD)
		return (store_pipe_in_left_child_if_needed(pipenode, mshcontext));
	else if (left_child && left_child->type == TK_PIPE)
		return (store_in_closest_left_if_needed(pipenode, mshcontext));
	return (EXIT_SUCCESS);
}

int	prepare_heredoc(t_astnode *lredir, t_shcontext *mshcontext)
{
	pid_t	pid;
	int		*pipedes;

	if (mshcontext->terminate)
		return (EXIT_FATAL);
	if (pipe(lredir->data.redirection.fd) < 0)
		return (mshcontext->terminate = true, EXIT_FATAL);
	pipedes = lredir->data.redirection.fd;
	pid = fork();
	if (pid < 0)
		return (close(pipedes[READ_END]), close(pipedes[WRITE_END]),
			perror("fork()"), mshcontext->terminate = true, EXIT_FATAL);
	else if (pid == 0)
		store_heredoc_input(lredir, pipedes, mshcontext);
	else
	{
		(signal(SIGINT, SIG_IGN), close(pipedes[WRITE_END]));
		waitpid(pid, &mshcontext->wstatus, 0);
		*(int *)mshcontext->envl->content = WEXITSTATUS(mshcontext->wstatus);
		if (WEXITSTATUS(mshcontext->wstatus) == 130)
			return(mshcontext->terminate = true, 130);
	}
	return (EXIT_SUCCESS);
}

void	visit_prematurely(t_astnode *node, t_shcontext *mshcontext)
{
	if (!node)
		return ;
	if (node->type == TK_LREDIR && node->data.redirection.mode == O_APPEND)
		prepare_heredoc(node, mshcontext);
	visit_prematurely(node->left, mshcontext);
	visit_prematurely(node->right, mshcontext);
}

static int	store_pipe_in_left_child_if_needed(t_astnode *pipenode,
		t_shcontext *mshcontext)
{
	t_astnode	*left_child;

	left_child = pipenode->left;
	if (pipe(left_child->data.command.fd) < 0)
		return (perror("pipe()"), mshcontext->terminate = true, EXIT_FATAL);
	left_child->data.command.thereispipe = true;
	if (pipenode->right)
	{
		pipenode->right->data.command.thereisprev = true;
		pipenode->right->data.command.prevfd = left_child->data.command.fd;
		pipenode->data.pipe.thereisinput = true;
	}
	return (EXIT_SUCCESS);
}

static int	store_in_closest_left_if_needed(t_astnode *pipenode,
		t_shcontext *mshcontext)
{
	t_astnode	*left_child;
	t_astnode	*closest_left;

	left_child = pipenode->left;
	closest_left = left_child->right;
	if (pipe(closest_left->data.command.fd) < 0)
		return (perror("pipe()"), mshcontext->terminate = true, EXIT_FATAL);
	closest_left->data.command.thereispipe = true;
	if (pipenode->right)
	{
		pipenode->right->data.command.thereisprev = true;
		pipenode->right->data.command.prevfd = closest_left->data.command.fd;
		pipenode->data.pipe.thereisinput = true;
	}
	return (EXIT_SUCCESS);
}
