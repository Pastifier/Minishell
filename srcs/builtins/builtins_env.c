/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 17:21:21 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/20 09:56:28 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include <stdio.h>

t_node		*find_variable(t_node **envp, const char *variable);
static int	perform_declare(t_node *iter);

int	env(t_node **envp, bool declare_flag)
{
	t_node	*iter;

	if (!envp)
		return (EXIT_SUCCESS);
	iter = *envp;
	if (declare_flag)
		return (perform_declare(iter));
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
	content = perform_joining_of_var_and_val(variable, value);
	if (!content)
		return (EXIT_FATAL);
	// Check for visibility of the variable. If it is not visible, then set it to visible.
	if (varp)
		return (free(varp->content), varp->content = content, EXIT_SUCCESS);
	to_append = node_create(content);
	if (!to_append)
		return (free(content), EXIT_FATAL);
	free(content);
	if (val_length == 0)
		to_append->visible = false;
	list_append(envp, to_append);
	return (EXIT_SUCCESS);
}

int	unset(t_node **envp, const char *variable)
{
	t_node	*varp;

	if (!variable)
		return (EXIT_SUCCESS);
	if (ft_strchr(variable, '='))
		return (EXIT_SUCCESS);
	varp = find_variable(envp, variable);
	node_destroy(varp);
	return (EXIT_SUCCESS);
}

t_node	*find_variable(t_node **envp, const char *variable)
{
	size_t	var_length;
	size_t	con_length;
	char	*eql_address;
	t_node	*iter;

	if (!envp && !*envp)
		return (NULL);
	var_length = ft_strlen(variable);
	iter = *envp;
	while (iter)
	{
		eql_address = ft_strchr(iter->content, '=');
		con_length = ft_strlen(iter->content);
		if (con_length >= var_length
			&& !ft_strncmp(iter->content, variable,
				ft_strlen(iter->content) - ft_strlen(eql_address)))
			return (iter);
		iter = iter->next;
	}
	return (NULL);
}

static int	perform_declare(t_node *iter)
{
	char	*variable;
	char	*eql_address;

	while (iter)
	{
		eql_address = ft_strchr(iter->content, '=');
		if (iter->content)
		{
			if (eql_address)
				variable = ft_substr(iter->content, 0,
						eql_address - (char *)iter->content + 1);
			else
				variable = ft_strdup(iter->content);
			if (!variable)
				return (EXIT_FATAL);
			printf("declare -x %s", variable);
			free(variable);
			if (eql_address)
				printf("\"%s\"\n", eql_address + 1);
		}
		iter = iter->next;
		if (!iter)
			write(STDOUT_FILENO, "\n", 1);
	}
	return (EXIT_SUCCESS);
}
