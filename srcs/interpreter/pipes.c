/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 23:40:22 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/16 03:11:35 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"
#include <stdio.h>

extern int	g_signal;

static int	fork_for_left_child_and_commit(t_astnode *left, t_node *envl);
static int	fork_for_right_child_and_commit(t_astnode *right, t_node *envl);

int	handle_pipe(t_astnode *pipenode, t_node *envl)
{
	int	*status;
	int	fork_failed;

	if (pipenode->type != TK_PIPE)
		return (EXIT_NEEDED);
	status = &pipenode->right->data.command.exit;
	fork_failed = fork_for_left_child_and_commit(pipenode->left, envl);
	if (fork_failed)
		return (EXIT_FATAL);
	fork_failed = fork_for_right_child_and_commit(pipenode->right, envl);
	if (fork_failed)
		return (EXIT_FATAL);
	if (WIFSIGNALED(*status))
	{
		// TODO:
		// 	print the corresponding signal to STDOUT;
		//	change g_signal to the corresponding signal;
		//	remember that signals get sent to both the child and parent, so do something to avoid duplicate behaviours!
	}
	return (WEXITSTATUS(pipenode->right->data.command.exit));
}

static int	fork_for_left_child_and_commit(t_astnode *left, t_node *envl)
{
	pid_t		pid;

	while (left->type != TK_WORD)
		left = left->right;
	pid = fork();
	if (pid < 0)
		return (perror("fork()"), EXIT_FATAL);
	if (pid == 0)
	{
		close(left->data.command.fd[READ_END]);
		dup2(left->data.command.fd[WRITE_END], STDOUT_FILENO);
		wexecve(left, envl);
		close(left->data.command.fd[WRITE_END]);
		// destroy tree
		exit(EXIT_FAILURE);
	}
	else
		wait(&left->data.command.exit);
	return (EXIT_SUCCESS);
}

static int	fork_for_right_child_and_commit(t_astnode *right, t_node *envl)
{
	pid_t		pid;
	t_astnode	*sibling;

	sibling = right->parent->left;
	while (sibling->type != TK_WORD)
		sibling = sibling->right;
	pid = fork();
	if (pid < 0)
		return (perror("fork()"), EXIT_FATAL);
	if (pid == 0)
	{
		close(sibling->data.command.fd[WRITE_END]);
		dup2(sibling->data.command.fd[READ_END], STDIN_FILENO);
		if (right->data.command.thereispipe)
			dup2(right->data.command.fd[WRITE_END], STDOUT_FILENO);
		wexecve(right, envl);
		close(sibling->data.command.fd[READ_END]);
		if (sibling->data.command.thereispipe)
		{
			close(right->data.command.fd[READ_END]);
			close(right->data.command.fd[WRITE_END]);
		}
		// Perhaps make a function that closes fds and destroys literally everything
	}
	else
	{
		close(sibling->data.command.fd[WRITE_END]);
		close(sibling->data.command.fd[READ_END]);
		wait(&right->data.command.exit);
	}
	return (EXIT_SUCCESS);
}
