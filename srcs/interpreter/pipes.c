/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 23:40:22 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/14 08:15:14 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal;

static void	fork_for_left_child_and_commit(t_astnode *left, t_node *envl);
static void	fork_for_right_child_and_commit(t_astnode *right, t_node *envl);

int	handle_pipe(t_astnode *pipenode, t_node *envl)
{
	int			*status;

	status = &pipenode->right->data.command.exit;
	fork_for_left_child_and_commit(pipenode->left, envl);
	fork_for_right_child_and_commit(pipenode->right, envl);
	if (WIFSIGNALED(*status))
	{
		// TODO:
		// 	print the corresponding signal to STDOUT;
		//	change g_signal to the corresponding signal;
		//	remember that signals get sent to both the child and parent, so do something to avoid duplicate behaviours!
	}
	return (WEXITSTATUS(pipenode->right->data.command.exit));
}

static void	fork_for_left_child_and_commit(t_astnode *left, t_node *envl)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork()"));
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
}

static void	fork_for_right_child_and_commit(t_astnode *right, t_node *envl)
{
	pid_t	pid;
	int		*left_pipefd;

	pid = fork();
	if (pid < 0)
		return (perror("fork()"));
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
		wait(&right->data.command.exit);
		close(left_pipefd[READ_END]);
		if (right->data.command.thereispipe)
			close(right->data.command.fd[WRITE_END]);
	}
}
