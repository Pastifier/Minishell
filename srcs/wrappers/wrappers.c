/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:54:54 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/30 19:24:04 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include <stdio.h>

static char	**clean_up_paths(char **paths);

void	check_perms(char *slash, char **args, char **envp, bool *keep_checking)
{
	if (!slash)
	{
		execve(args[0], args, envp);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	else if (access(args[0], X_OK))
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		*keep_checking = false;
	}
	else if (slash)
	{
		execve(args[0], args, envp);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	}
	else
	{
		execve(args[0], args, envp);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
}

void	search_path_execute(char **envp, bool *keep_checking, t_split paths,
		char **args)
{
	char	*temp;
	char	*slash;

	slash = ft_strchr(args[0], '/');
	while (!slash && paths.array && *paths.array)
	{
		temp = ft_strjoin(*paths.array, args[0]);
		if (!access(temp, F_OK) && *keep_checking)
		{
			if (!access(temp, X_OK))
			{
				execve(temp, args, envp);
				*keep_checking = false;
			}
			else
			{
				ft_putstr_fd("msh: ", STDERR_FILENO);
				ft_putstr_fd(args[0], STDERR_FILENO);
				ft_putendl_fd(": Permission denied", STDERR_FILENO);
				*keep_checking = false;
			}
		}
		(free(temp), paths.array++);
	}
}

int	closing_remarks(t_wexec *wexec, char **envp)
{
	if (!access(wexec->args[0], F_OK) && wexec->keep_checking)
		check_perms(wexec->slash, wexec->args, envp, &wexec->keep_checking);
	else
	{
		ft_putstr_fd(wexec->args[0], STDERR_FILENO);
		write(STDERR_FILENO, ": command not found\n", 20);
		wexec->keep_checking = true;
	}
	str_arr_destroy(wexec->paths.array);
	str_arr_destroy(wexec->args);
	if (wexec->dir)
		closedir(wexec->dir);
	if (!wexec->keep_checking)
		return (126);
	return (127);
}

int	wexecve(t_astnode *word, t_node *envl, char **envp)
{
	t_node	*pathnode;
	t_wexec	wexec;

	wexec.args = list_cpy_to_str_arr(word->data.command.args);
	if (!wexec.args)
		return (EXIT_FATAL);
	wexec.slash = ft_strchr(wexec.args[0], '/');
	wexec.dir = opendir(wexec.args[0]);
	if (wexec.dir && wexec.slash)
		return (handle_slash_is_dir(wexec.args, wexec.dir));
	else if (!wexec.dir && wexec.slash)
	{
		if (access(wexec.args[0], F_OK))
			return (handle_slash_not_dir(wexec.args));
	}
	pathnode = find_variable(&envl, "PATH=");
	if (!pathnode)
		return (handle_no_pathnode(wexec.args, envp));
	wexec.paths = ft_split(pathnode->content, ":");
	if (!clean_up_paths(wexec.paths.array))
		return (str_arr_destroy(wexec.paths.array), str_arr_destroy(envp),
			str_arr_destroy(wexec.args), EXIT_FATAL);
	wexec.keep_checking = true;
	search_path_execute(envp, &wexec.keep_checking, wexec.paths, wexec.args);
	return (closing_remarks(&wexec, envp));
}

static char	**clean_up_paths(char **paths)
{
	char	*temp;
	char	**dummy;
	char	*hold;

	if (!paths)
		return (NULL);
	temp = paths[0];
	paths[0] = ft_strdup(temp + 5);
	free(temp);
	if (!paths[0])
		return (str_arr_destroy(paths), NULL);
	dummy = paths - 1;
	while (*++dummy)
	{
		if ((*dummy)[ft_strlen(*dummy) - 1] != '/')
		{
			temp = *dummy;
			hold = ft_strjoin(*dummy, "/");
			free(temp);
			if (!hold)
				return (NULL);
			*dummy = hold;
		}
	}
	return (paths);
}
