/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:54:54 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/16 07:21:42 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static char	**clean_up_paths(char **paths);

int		wexecve(t_astnode *word, t_node *envl, char **envp)
{
	t_split	paths;
	void	*temp;
	char	*slash;
	t_node	*pathnode;

	slash = ft_strchr(word->data.command.args[0], '/');
	pathnode = find_variable(&envl, "PATH=");
	temp = NULL;
	if (pathnode)
		temp = pathnode->content;
	paths = ft_split(temp, ":");
	if (!clean_up_paths(paths.array))
		return (str_arr_destroy(paths.array), free(envp), EXIT_FATAL);
	while (!slash && paths.array && *paths.array)
	{
		temp = ft_strjoin(*paths.array, word->data.command.args[0]);
		execve(temp, word->data.command.args, NULL);
		(free(temp), paths.array++);
	}
	execve(word->data.command.args[0], word->data.command.args, envp);
	(perror("bash"), str_arr_destroy(paths.array - paths.wordcount));
	free(envp);
	return (EXIT_FAILURE);
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
				return (str_arr_destroy(paths), NULL);
			*dummy = hold;
		}
	}
	return (paths);
}
