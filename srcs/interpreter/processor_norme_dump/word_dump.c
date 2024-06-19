/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_dump.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:47:30 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/19 07:25:24 by ebinjama         ###   ########.fr       */
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

int	wexit(t_astnode *word, t_shcontext *mshcontext, char *first_arg)
{
	if (first_arg && ft_atoi(first_arg).error)
	{
		ft_putstr_fd("exit\nmsh: exit: ", STDERR_FILENO);
		ft_putstr_fd(first_arg, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		list_destroy(&mshcontext->envl);
		str_arr_destroy(mshcontext->allocated_envp);
		destroy_ast(mshcontext->root);
		exit(EXIT_INVAL_ARG);
	}
	if (first_arg && word->data.command.args->next->next)
	{
		ft_putendl_fd("exit\nmsh: exit: too many arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (first_arg)
		mshcontext->exit_status = ft_atoi(first_arg).value;
	list_destroy(&mshcontext->envl);
	str_arr_destroy(mshcontext->allocated_envp);
	destroy_ast(mshcontext->root);
	exit(mshcontext->exit_status);
	return (EXIT_SUCCESS);
}

void	init_builtin_necessities(t_astnode *word, char **variable, char **temp,
			char **first_arg)
{
	*variable = NULL;
	*temp = NULL;
	*first_arg = NULL;
	if (word->data.command.args->next)
		*first_arg = word->data.command.args->next->content;
}

int	w_wexport(t_astnode *word, t_node **envp)
{
	t_node	*args;
	char	*eql_addr;
	char	*variable;
	char	*value;

	args = word->data.command.args->next;
	if (!args)
		return (env(envp, true));
	value = NULL;
	while (args)
	{
		eql_addr = ft_strchr((char *)args->content, '=');
		if (eql_addr)
			value = eql_addr + 1;
		variable = ft_substr((char *)args->content, 0,
				eql_addr - (char *)args->content);
		if (!variable)
			return (EXIT_FAILURE);
		if (parse_export(variable))
			return (free(variable), EXIT_FAILURE);
		if (bltin_export(envp, variable, value))
			return (free(variable), EXIT_FAILURE);
		(free(variable), args = args->next);
	}
	return (EXIT_SUCCESS);
}
