/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:54:54 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/23 18:28:35 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <stdio.h>

static char	**clean_up_paths(char **paths);

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

void	check_perms_and_display_error(char *slash, char **args, char **envp,
	bool *keep_checking)
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

void	search_path_and_execute_if_found(char **envp, bool *keep_checking,
		t_split *paths, char **args)
{
	char	*temp;
	char	*slash;

	slash = ft_strchr(args[0], '/');
	while (!slash && paths->array && *paths->array)
	{
		temp = ft_strjoin(*paths->array, args[0]);
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
		(free(temp), (paths->array)++);
	}
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

int	wexecve(t_astnode *word, t_node *envl, char **envp)
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
	if (dir && slash)
		return (handle_slash_is_dir(args, dir));
	else if (!dir && slash)
	{
		if (access(args[0], F_OK))
			return (handle_slash_not_dir(args));
	}
	pathnode = find_variable(&envl, "PATH=");
	if (!pathnode)
		return (handle_no_pathnode(args, envp));
	temp = NULL;
	if (pathnode)
		temp = pathnode->content;
	paths = ft_split(temp, ":");
	if (!clean_up_paths(paths.array))
		return (str_arr_destroy(paths.array), str_arr_destroy(envp),
			str_arr_destroy(args), EXIT_FATAL);
	keep_checking = true;
	search_path_and_execute_if_found(envp, &keep_checking, &paths, args);
	if (keep_checking)
	{
		if (!access(args[0], F_OK) && keep_checking)
			check_perms_and_display_error(slash, args, envp, &keep_checking);
		else
		{
			ft_putstr_fd(args[0], STDERR_FILENO);
			write(STDERR_FILENO, ": command not found\n", 20);
		}
	}
	else
	{
		ft_putstr_fd(args[0], STDERR_FILENO);
		write(STDERR_FILENO, ": command not found\n", 20);
		keep_checking = true;
	}
	if (!slash)
		str_arr_destroy(paths.array - paths.wordcount);
	else
		str_arr_destroy(paths.array);
	str_arr_destroy(args);
	if (dir)
		closedir(dir);
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
