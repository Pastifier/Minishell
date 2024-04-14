/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 08:29:40 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/14 10:28:13 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	execute_word_leaf_node(t_astnode *word, t_node *envl);
// CHECKS WHETHER THE WORD NEEDS TO BE PIPED OR WHATNOT
// MIGHT FILL IT WITH OTHER INFO SUCH AS THE FD OF THE FILE
// (IF IT IS REFERRING TO ONE).
// ASSUMES THAT A WORD CAN NEVER HAVE A WORD-PARENT
int	handle_word(t_astnode *word, t_node *envl)
{
	if (!word->parent)
		word->data.command.exit = execute_word_leaf_node(word, envl);
	else if (word == word->parent->right)
		return (EXIT_SUCCESS);
	else if (word->parent->type == TK_PIPE)
		if (pipe(word->data.command.fd) < 0)
			return ((word->data.command.exit = EXIT_FAILURE));
	else if (word->parent->parent
		&& (word->parent->parent->type == TK_PIPE
			|| word->parent->parent->type == TK_RREDIR))
		if (pipe(word->data.command.fd) < 0)
			return ((word->data.command.exit = EXIT_FAILURE));
	return (word->data.command.exit);
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
}
