/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret_dump.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 15:12:42 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/21 09:12:20 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

extern volatile sig_atomic_t	g_signal;

void	perform_wait_and_fetch_wstatus(t_shcontext *mshcontext)
{
	int		fetch;
	bool	keep_printing;

	fetch = 1;
	keep_printing = true;
	while (fetch > 0 && mshcontext->rightmost_word)
	{
		fetch = wait(&mshcontext->wstatus);
		if (keep_printing && WIFSIGNALED(mshcontext->wstatus) && fetch > 0)
		{
			if (WTERMSIG(mshcontext->wstatus) == SIGINT)
				write(1, "\n", 1);
			else if (WTERMSIG(mshcontext->wstatus) == SIGQUIT)
				write(1, "No one appreciates a quitter.\n", 31);
			else if (WTERMSIG(mshcontext->wstatus) == SIGSEGV)
				ft_putendl_fd(SV SV2, STDERR_FILENO);
			keep_printing = false;
		}
		if (fetch == mshcontext->rightmost_word->data.command.pid)
			mshcontext->exit_status = mshcontext->wstatus;
	}
}

int	determine_exit_code(t_shcontext *mshcontext)
{
	int	*exit_code;

	exit_code = (int *)mshcontext->envl->content;
	if (WIFSIGNALED(mshcontext->exit_status))
	{
		g_signal = WTERMSIG(mshcontext->exit_status);
		return (*(int *)mshcontext->envl->content = g_signal + 128);
	}
	if (!mshcontext->permissions_clear)
		return (*(int *)mshcontext->envl->content = 1);
	if (mshcontext->rightmost_word
		&& mshcontext->rightmost_word->data.command.builtin)
		return (*(int *)mshcontext->envl->content);
	if (mshcontext->rightmost_word
		&& mshcontext->rightmost_word->data.command.execute)
	{
		*exit_code = WEXITSTATUS(mshcontext->exit_status);
		return (WEXITSTATUS(mshcontext->exit_status));
	}
	if (mshcontext->terminate)
		return (EXIT_FAILURE);
	return (EXIT_FAILURE);
}
