/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:54:54 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/14 13:38:36 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int		wexecve(t_astnode *word, t_node *envl)
{
	t_split	paths;
	char	**envp;
	char	*temp;
	char	*slash;

	envp = list_cpy_to_str_arr(envl);
	slash = ft_strchr(word->data.command.args[0], '/');
	paths = ft_split(find_variable(&envl, "PATH=")->content, ":");
	while (!slash && paths.array && *paths.array)
	{
		temp = ft_strjoin(*paths.array, word->data.command.args[0]);
		execve(temp, word->data.command.args, envp);
		free(temp);
		paths.array++;
		if (!*paths.array)
			return (str_arr_destroy(paths.array - paths.wordcount),
					free(envp), perror(word->data.command.args[0]),
					EXIT_FAILURE);
	}
	execve(word->data.command.args[0], word->data.command.args, envp);
	perror("execve");
	str_arr_destroy(paths.array);
	free(envp);
	return (EXIT_FAILURE);
}
