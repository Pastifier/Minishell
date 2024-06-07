/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 08:29:40 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/07 19:14:29 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"
#include <stdio.h>

extern int	g_signal;

static int	execute_word_leaf_node(t_astnode *word, t_node *envl, t_shcontext *mshcontext);
static int	execute_builtin(t_astnode *word, t_shcontext *mshcontext);
static bool	is_builtin(t_astnode *word, t_shcontext *mshcontext);

int	handle_word(t_astnode *word, t_node *envl, t_shcontext *mshcontext)
{

	if (word->type != TK_WORD)
		return (EXIT_NEEDED);
	if (is_builtin(word, mshcontext) && !word->parent)
		*(int*)mshcontext->envl->content = execute_builtin(word, mshcontext);
	else
		execute_word_leaf_node(word, envl, mshcontext);
	return (WEXITSTATUS(word->data.command.exit));
}

int	execute_word_leaf_node(t_astnode *word, t_node *envl, t_shcontext *mshcontext)
{
	pid_t		pid;
	int			fetch;
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
		if (word->data.command.builtin)
			exit(execute_builtin(word, mshcontext));
		else if (word->data.command.execute)
			fetch = wexecve(word, envl, envp);
		(str_arr_destroy(envp), list_destroy(&envl));
		// destroy stuff.
		exit(fetch);
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

static int	execute_builtin(t_astnode *word, t_shcontext *mshcontext)
{
	char		*first_arg;
	char		*temp;
	char		*variable;
	int			fetch;
	char		*cmd;

	cmd = word->data.command.args->content;
	first_arg = NULL;
	temp = NULL;
	variable = NULL;
	if (word->data.command.args->next)
		first_arg = word->data.command.args->next->content;
	if (!ft_strncmp(cmd, "cd", -1))
		return (wcd(word, mshcontext));
	if (!ft_strncmp(cmd, "env", -1))
		return (env(&mshcontext->envl, false));
	if (!ft_strncmp(cmd, "pwd", -1))
		return (pwd());
	if (!ft_strncmp(cmd, "unset", -1))
		return (unset(&mshcontext->envl, first_arg));
	if (!ft_strncmp(cmd, "echo", -1))
		return (echo(word, word->data.command.args->next));
	if (!ft_strncmp(cmd, "export", -1))
	{
		if (!first_arg)
			return (env(&mshcontext->envl, true));
		temp = ft_strchr(first_arg, '=');
		if (temp)
		{
			variable = ft_substr(first_arg, 0, temp - first_arg + 1);
			if (!variable)
				return (EXIT_FATAL);
			fetch = bltin_export(&mshcontext->envl, variable, temp + 1);
			return (free(variable), fetch);
		}
		return (bltin_export(&mshcontext->envl, first_arg, ""));
	}
	return (EXIT_FATAL);
}
