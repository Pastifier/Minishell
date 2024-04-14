/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 08:29:40 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/14 12:49:42 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

extern int	g_signal;

int	execute_word_leaf_node(t_astnode *word, t_node *envl);

int	handle_word(t_astnode *word, t_node *envl)
{
	if (!word->parent)
		return (execute_word_leaf_node(word, envl));
	else if (word == word->parent->right)
		return (EXIT_SUCCESS);
	else if (word->parent->type == TK_PIPE)
	{
		if (pipe(word->data.command.fd) < 0)
			return ((word->data.command.exit = EXIT_FAILURE));
	}
	else if (word->parent->parent
		&& (word->parent->parent->type == TK_PIPE
			|| word->parent->parent->type == TK_RREDIR))
		if (pipe(word->data.command.fd) < 0)
			return ((word->data.command.exit = EXIT_FAILURE));
	return (WEXITSTATUS(word->data.command.exit));
}

int	execute_word_leaf_node(t_astnode *word, t_node *envl)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork()"), EXIT_FATAL);
	if (pid == 0)
		wexecve(word, envl);
	else
		wait(&word->data.command.exit);
	if (WIFSIGNALED(word->data.command.exit))
	{
		// TODO:
		// 	print the corresponding signal to STDOUT;
		//	change g_signal to the corresponding signal;
		//	remember that signals get sent to both the child and parent, so do something to avoid duplicate behaviours!
	}
	return (EXIT_SUCCESS);
}
