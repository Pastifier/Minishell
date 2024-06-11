/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 17:21:21 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/12 02:42:13 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*find_variable(t_node **envp, const char *variable);

int	env(t_node **envp, bool declare_flag)
{
	t_node	*iter;
	char	*variable;
	char	*eql_address;

	if (!envp)
		return (EXIT_SUCCESS);
	iter = *envp;
	if (declare_flag)
	{
		while (iter)
		{
			eql_address = ft_strchr(iter->content, '=');
			if (iter->content)
			{
				if (eql_address)
					variable = ft_substr(iter->content, 0, eql_address - (char *)iter->content + 1);
				else
					variable = ft_strdup(iter->content);
				if (!variable)
					return (EXIT_FATAL);
				ft_putstr_fd("declare -x ", STDOUT_FILENO);
				ft_putstr_fd(variable, STDOUT_FILENO);
				free(variable);
				if (eql_address)
				{
					ft_putstr_fd("\"", STDOUT_FILENO);
					ft_putstr_fd(eql_address + 1, STDOUT_FILENO);
					ft_putstr_fd("\"", STDOUT_FILENO);
				}
				ft_putstr_fd("\n", STDOUT_FILENO);
			}
			iter = iter->next;
		}
	}
	else
	{
		while (iter)
		{
			if (iter->content && iter->visible)
				ft_putendl_fd(iter->content, STDOUT_FILENO);
			iter = iter->next;
		}
	}
	return (EXIT_SUCCESS);
}

int	bltin_export(t_node **envp, const char *variable, const char *value)
{
	char	*content;
	size_t	var_length;
	size_t	val_length;
	t_node	*to_append;
	t_node	*varp;

	var_length = ft_strlen(variable);
	val_length = ft_strlen(value);
	varp = find_variable(envp, variable);
	if (varp && ft_strlen(varp->content) - var_length == val_length
		&& !ft_strncmp((char *)varp->content + var_length, value, val_length))
		return (EXIT_SUCCESS);
	content = ft_strjoin(variable, value);
	if (!content)
		return (EXIT_FATAL);
	if (varp)
		return (free(varp->content), varp->content = content, EXIT_SUCCESS);
	to_append = node_create(content);
	if (!to_append)
		return (free(content), EXIT_FATAL);
	if (val_length == 0)
		to_append->visible = false;
	list_append(envp, to_append);
	return (EXIT_SUCCESS);
}

int	unset(t_node **envp, const char *variable)
{
	t_node	*varp;

	if (!variable)
		return (EXIT_NEEDED);
	varp = find_variable(envp, variable);
	node_destroy(varp);
	return (EXIT_SUCCESS);
}

t_node	*find_variable(t_node **envp, const char *variable)
{
	size_t	var_length;
	size_t	con_length;
	t_node	*iter;

	if (!envp && !*envp)
		return (NULL);
	var_length = ft_strlen(variable);
	iter = *envp;
	while (iter)
	{
		con_length = ft_strlen(iter->content);
		if (con_length >= var_length
			&& !ft_strncmp(iter->content, variable, var_length))
			return (iter);
		iter = iter->next;
	}
	return (NULL);
}
