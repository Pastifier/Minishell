/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper_dump.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:10:40 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/25 17:06:49 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

int	handle_slash_is_dir(char **args, DIR *dir)
{
	write(STDERR_FILENO, "msh: ", 6);
	ft_putstr_fd(args[0], STDERR_FILENO);
	write(STDERR_FILENO, ": is a directory\n", 18);
	(str_arr_destroy(args), closedir(dir));
	return (126);
}

int	handle_slash_not_dir(char **args)
{
	write(STDERR_FILENO, "msh: ", 6);
	ft_putstr_fd(args[0], STDERR_FILENO);
	write(STDERR_FILENO, ": No such file or directory\n", 28);
	(str_arr_destroy(args));
	return (127);
}

int	handle_no_pathnode(char **args, char **envp)
{
	execve(args[0], args, envp);
	write(STDERR_FILENO, "msh: ", 6);
	ft_putstr_fd(args[0], STDERR_FILENO);
	write(STDERR_FILENO, ": command not found\n", 20);
	(str_arr_destroy(args));
	return (127);
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

int	wexit(t_astnode *word, t_shcontext *mshcontext, char *first_arg)
{
	if (first_arg && ft_atoi(first_arg).error)
	{
		ft_putstr_fd("exit\nmsh: exit: ", STDERR_FILENO);
		ft_putstr_fd(first_arg, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		list_destroy(&mshcontext->envl);
		str_arr_destroy(mshcontext->allocated_envp);
		(destroy_ast(mshcontext->root), restore_iodes(mshcontext, true));
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
	(destroy_ast(mshcontext->root), restore_iodes(mshcontext, true));
	exit(mshcontext->exit_status);
	return (EXIT_SUCCESS);
}
