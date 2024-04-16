/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 08:29:40 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/16 01:57:17 by ebinjama         ###   ########.fr       */
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
	if (word->parent)
		return (EXIT_NEEDED);
	execute_word_leaf_node(word, envl);
	return (WEXITSTATUS(word->data.command.exit));
}

int	execute_word_leaf_node(t_astnode *word, t_node *envl)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork()"), EXIT_FATAL);
	if (pid == 0)
	{
		wexecve(word, envl);
		// destroy stuff.
	}
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
