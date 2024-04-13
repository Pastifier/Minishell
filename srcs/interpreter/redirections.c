/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:01:57 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/13 10:42:01 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static int	child_process(t_astnode *node, int pipefd[2], int infile,
				t_node *envl);
static int	parent_process(t_astnode *node, int pipefd[2], int infile);
static int	write_infile_to_pipe(int pipefd[2], int infile);
static int	pipe_command_fd(int infile);

int	handle_lredir(const char *filename, t_astnode *node, t_node *envl)
{
	pid_t	pid;
	int		pipefd[2];
	int		infile;
	int		exitcode;

	if (!node || !filename)
		return (EXIT_FATAL);
	infile = open(filename, O_RDONLY);
	if (infile < 2)
		return (perror(filename), EXIT_FATAL);
	if (pipe(pipefd) == -1)
		return (perror("pipe"), EXIT_FATAL);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), EXIT_FATAL);
	if (pid == 0)
		exitcode = child_process(node, pipefd, infile, envl);
	else
		exitcode = parent_process(node, pipefd, envl);
	return (exitcode);
}

static int	parent_process(t_astnode *node, int pipefd[2], int infile)
{
	int	infd;

	infd = dup(STDIN_FILENO);
	if (node->left)
	{
		if (node->parent && node->parent->type == TK_PIPE)
		{
			if (pipe(node->left->data.command.fd) < 0)
				return (EXIT_FATAL);
			// overwriting STDOUT is bad! Fix this.
			dup2(node->left->data.command.fd[WRITE_END], STDOUT_FILENO);
		}
		write_infile_to_pipe(pipefd, infile);
		wait(&node->left->data.command.exit);
		return (close(infile), EXIT_SUCCESS);
	}
	if (node->parent->type == TK_WORD)
		if (pipe(node->parent->data.command.fd) < 0)
			return (close(infile), EXIT_FATAL);
	// overwriting STDIN is bad! Fix this.
	(close(pipefd[WRITE_END]), dup2(pipefd[READ_END], STDIN_FILENO));
	(close(pipefd[READ_END]), close(infile), wait(NULL));
	return (infd);
}

static int child_process(t_astnode *node, int pipefd[2], int infile,
						 t_node *envl)
{
	if (node->left)
	{
		close(pipefd[WRITE_END]);
		dup2(pipefd[READ_END], STDIN_FILENO);
		close(pipefd[READ_END]);
		close(infile);
		return (wexecve(node->left, envl));
	}
	write_infile_to_pipe(pipefd, infile);
	close(infile);
	return (EXIT_SUCCESS);
}

static int write_infile_to_pipe(int pipefd[2], int infile)
{
	ssize_t bytes;
	char buffer[42];

	bytes = 1;
	close(pipefd[READ_END]);
	while (bytes > 0)
	{
		bytes = read(infile, buffer, 42);
		if (bytes < 0)
			return (perror("read"), EXIT_FATAL);
		write(pipefd[WRITE_END], buffer, bytes);
	}
	close(pipefd[WRITE_END]);
	return (EXIT_SUCCESS);
}
