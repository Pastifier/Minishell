/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_dump.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:47:30 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/18 21:13:04 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

void	perform_word_checks_and_close_pipes_if_needed(t_astnode *word,
		t_shcontext *mshcontext, char **envp, t_node *envl)
{
	int	fetch;

	fetch = EXIT_FAILURE;
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
	if (word->data.command.builtin && word->data.command.execute)
	{
		mshcontext->allocated_envp = envp;
		fetch = execute_builtin(word, mshcontext);
		(str_arr_destroy(envp), destroy_ast(mshcontext->root));
		list_destroy(&envl);
		restore_iodes(mshcontext, true);
		exit(fetch);
	}
}

void	ignore_signals_and_close_pipes_if_needed_then_set_pid(t_astnode *word,
		pid_t pid, char **envp)
{
	(signal(SIGINT, SIG_IGN), str_arr_destroy(envp));
	if (word->data.command.thereisprev)
		close(word->data.command.prevfd[READ_END]);
	if (word->data.command.thereispipe)
		close(word->data.command.fd[WRITE_END]);
	word->data.command.pid = pid;
}
