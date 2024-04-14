/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 23:40:22 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/14 15:05:54 by aalshafy         ###   ########.fr       */
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

	status = &pipenode->right->data.command.exit;
	fork_failed = fork_for_left_child_and_commit(pipenode->left, envl);
	if (fork_failed)
		return ((pipenode->left->data.command.exit = EXIT_FATAL));
	fork_failed = fork_for_right_child_and_commit(pipenode->right, envl);
	if (fork_failed)
		return ((pipenode->right->data.command.exit = EXIT_FATAL));
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
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork()"), EXIT_FATAL);
	if (pid == 0)
	{
		close(left->data.command.fd[READ_END]);
		dup2(left->data.command.fd[WRITE_END], STDOUT_FILENO);
		wexecve(left, envl);
		close(left->data.command.fd[WRITE_END]);
	}
	else
	{
		wait(&left->data.command.exit);
		close(left->data.command.fd[WRITE_END]);
	}
	return (EXIT_SUCCESS);
}

static int	fork_for_right_child_and_commit(t_astnode *right, t_node *envl)
{
	pid_t	pid;
	int		*left_pipefd;

	pid = fork();
	if (pid < 0)
		return (perror("fork()"), EXIT_FATAL);
	left_pipefd = right->parent->left->data.command.fd;
	if (pid == 0)
	{
		dup2(left_pipefd[READ_END], STDIN_FILENO);
		close(left_pipefd[READ_END]);
		if (right->data.command.thereispipe)
			(close(right->data.command.fd[READ_END]),
				dup2(right->data.command.fd[WRITE_END], STDOUT_FILENO));
		wexecve(right, envl);
		if (right->data.command.thereispipe)
			close(right->data.command.fd[WRITE_END]);
	}
	else
	{
		(wait(&right->data.command.exit), close(left_pipefd[READ_END]));
		if (right->data.command.thereispipe)
			close(right->data.command.fd[WRITE_END]);
	}
	return (EXIT_SUCCESS);
}
