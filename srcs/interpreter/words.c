/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 08:29:40 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/03 15:42:11 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"
#include <stdio.h>

extern int	g_signal;

int	execute_word_leaf_node(t_astnode *word, t_node *envl);

int	handle_word(t_astnode *word, t_node *envl)
{

	if (word->type != TK_WORD)
		return (EXIT_NEEDED);
	execute_word_leaf_node(word, envl);
	return (WEXITSTATUS(word->data.command.exit));
}

int	execute_word_leaf_node(t_astnode *word, t_node *envl)
{
	pid_t		pid;
	char		**envp;

	envp = list_cpy_to_str_arr(envl);
	if (!envp)
		return (EXIT_FATAL);
	pid = fork();
	if (pid < 0)
		return ((void)write(2, "msh: ", 5), perror("fork()"), EXIT_FATAL);
	if (pid == 0)
	{
		if (word->data.command.thereisprev)
		{
			if (!word->data.command.thereisin && word->data.command.execute)
				dup2(word->data.command.prevfd[READ_END], STDIN_FILENO);
			close(word->data.command.prevfd[READ_END]);
		}
		if (word->data.command.thereispipe)
		{
			close(word->data.command.fd[READ_END]);
			if (!word->data.command.thereisout && word->data.command.execute)
				dup2(word->data.command.fd[WRITE_END], STDOUT_FILENO);
			close(word->data.command.fd[WRITE_END]);
		}
		if (word->data.command.execute)
			wexecve(word, envl, envp);
		(str_arr_destroy(envp), list_destroy(&envl));
		// destroy stuff.
		exit(EXIT_FAILURE);
	}
	else
	{
		str_arr_destroy(envp);
		// temp solution.
		if (word->data.command.thereisprev)
			close(word->data.command.prevfd[READ_END]);
		if (word->data.command.thereispipe)
			close(word->data.command.fd[WRITE_END]);
		if (word->data.command.thereisout)
			close(word->data.command.outfd);
		word->data.command.pid = pid;
	}
	return (EXIT_SUCCESS);
}
