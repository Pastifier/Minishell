/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:54:54 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/17 00:32:26 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <dirent.h>

static char	**clean_up_paths(char **paths);

int		wexecve(t_astnode *word, t_node *envl, char **envp)
{
	t_split	paths;
	DIR		*dir;
	void	*temp;
	char	*slash;
	bool	keep_checking;
	t_node	*pathnode;
	char	**args;

	args = list_cpy_to_str_arr(word->data.command.args);
	if (!args)
		return (EXIT_FATAL);
	slash = ft_strchr(args[0], '/');
	dir = opendir(args[0]);
	if (dir)
	{
		write(STDERR_FILENO, "msh: ", 6);
		ft_putstr_fd(args[0], STDERR_FILENO);
		write(STDERR_FILENO, ": is a directory\n", 18);
		(str_arr_destroy(args), closedir(dir));
		return (126);
	}
	else if (!dir && slash)
	{
		if (access(args[0], F_OK))
		{
			write(STDERR_FILENO, "msh: ", 6);
			ft_putstr_fd(args[0], STDERR_FILENO);
			write(STDERR_FILENO, ": No such file or directory\n", 29);
			(str_arr_destroy(args));
			return (127);
		}
	}
	pathnode = find_variable(&envl, "PATH=");
	if (!pathnode)
	{
		execve(args[0], args, envp);
		write(STDERR_FILENO, "msh: ", 6);
		ft_putstr_fd(args[0], STDERR_FILENO);
		write(STDERR_FILENO, ": command not found\n", 20);
		(str_arr_destroy(args));
		return (127);
	}
	temp = NULL;
	if (pathnode)
		temp = pathnode->content;
	paths = ft_split(temp, ":");
	if (!clean_up_paths(paths.array))
		return (str_arr_destroy(paths.array), str_arr_destroy(envp), str_arr_destroy(args), EXIT_FATAL);
	keep_checking = true;
	while (!slash && paths.array && *paths.array)
	{
		temp = ft_strjoin(*paths.array, args[0]);
		if (!access(temp, F_OK) && keep_checking)
		{
			if (!access(temp, X_OK))
			{
				execve(temp, args, envp);
				keep_checking = false;
			}
			else
			{
				ft_putstr_fd("msh: ", STDERR_FILENO);
				ft_putstr_fd(args[0], STDERR_FILENO);
				ft_putendl_fd(": Permission denied", STDERR_FILENO);
				keep_checking = false;
			}
		}
		(free(temp), paths.array++);
	}
	if (keep_checking)
	{
		if (!access(args[0], F_OK) && keep_checking)
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
				keep_checking = false;
			}
			else
			{
				execve(args[0], args, envp);
				ft_putstr_fd(args[0], STDERR_FILENO);
				ft_putendl_fd(": command not found", STDERR_FILENO);
			}
		}
		else
		{
			ft_putstr_fd(args[0], STDERR_FILENO);
			write(STDERR_FILENO, ": command not found\n", 20);
		}
	}
	if (!slash)
		str_arr_destroy(paths.array - paths.wordcount);
	else
		str_arr_destroy(paths.array);
	str_arr_destroy(args);
	if (!keep_checking)
		return (126);
	return (127);
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
