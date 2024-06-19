/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:36:57 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/19 06:59:33 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"
#include <stdio.h>

extern volatile sig_atomic_t	g_signal;

static int	execute_word_leaf_node(t_astnode *word, t_node *envl,
				t_shcontext *mshcontext);
static bool	is_builtin(t_astnode *word, t_shcontext *mshcontext);
int			execute_builtin(t_astnode *word, t_shcontext *mshcontext);

int	handle_word(t_astnode *word, t_node *envl, t_shcontext *mshcontext)
{
	if (word->type != TK_WORD)
		return (EXIT_NEEDED);
	if (is_builtin(word, mshcontext) && !word->parent && !mshcontext->terminate)
		*(int *)mshcontext->envl->content = execute_builtin(word, mshcontext);
	else
		execute_word_leaf_node(word, envl, mshcontext);
	return (WEXITSTATUS(word->data.command.exit));
}

int	execute_word_leaf_node(t_astnode *word, t_node *envl,
		t_shcontext *mshcontext)
{
	pid_t		pid;
	int			fetch;
	char		**envp;

	envp = list_cpy_to_str_arr(envl->next);
	if (!envp)
		return (EXIT_FATAL);
	pid = fork();
	fetch = EXIT_FAILURE;
	if (pid < 0)
		return ((void)write(2, "msh: ", 5), perror("fork()"), EXIT_FATAL);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		perform_word_checks_and_close_pipes_if_needed(word, mshcontext,
			envp, envl);
		if (word->data.command.execute)
			fetch = wexecve(word, envl, envp);
		(destroy_ast(mshcontext->root), str_arr_destroy(envp));
		(list_destroy(&envl), restore_iodes(mshcontext, true), exit(fetch));
	}
	else
		ignore_signals_and_close_pipes_if_needed_then_set_pid(word, pid, envp);
	return (EXIT_SUCCESS);
}

static bool	is_builtin(t_astnode *word, t_shcontext *mshcontext)
{
	char	*cmd;

	(void)mshcontext;
	cmd = word->data.command.args->content;
	if (!ft_strncmp(cmd, "cd", -1)
		|| !ft_strncmp(cmd, "env", -1)
		|| !ft_strncmp(cmd, "pwd", -1)
		|| !ft_strncmp(cmd, "echo", -1)
		|| !ft_strncmp(cmd, "unset", -1)
		|| !ft_strncmp(cmd, "export", -1)
		|| !ft_strncmp(cmd, "exit", -1))
	{
		word->data.command.builtin = true;
		return (true);
	}
	return (false);
}

int	execute_builtin(t_astnode *word, t_shcontext *mshcontext)
{
	char		*first_arg;
	char		*temp;
	char		*variable;
	char		*cmd;

	cmd = word->data.command.args->content;
	init_builtin_necessities(word, &variable, &temp, &first_arg);
	if (!ft_strncmp(cmd, "cd", -1))
		return (wcd(word, mshcontext));
	if (!ft_strncmp(cmd, "env", -1))
		return (env(&(mshcontext->envl->next), false));
	if (!ft_strncmp(cmd, "pwd", -1))
		return (pwd());
	if (!ft_strncmp(cmd, "unset", -1))
		return (wunset(word, &mshcontext->envl));
	if (!ft_strncmp(cmd, "echo", -1))
		return (echo(word, word->data.command.args->next));
	if (!ft_strncmp(cmd, "export", -1))
		return (w_wexport(word, &mshcontext->envl));
	if (!ft_strncmp(cmd, "exit", -1))
		return (wexit(word, mshcontext, first_arg));
	return (EXIT_FATAL);
}
