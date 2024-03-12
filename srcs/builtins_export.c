/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 17:13:17 by ebinjama          #+#    #+#             */
/*   Updated: 2024/03/12 17:44:00 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export(t_node **envp, const char *variable, const char *value)
{
	char	*content;
	size_t	full_length;
	t_node	*to_append;

	full_length = ft_strlen(variable) + ft_strlen(value);
	content = malloc(sizeof(char) * (full_length + 3));
	if (!content)
		return (EXIT_FATAL);
	ft_strlcpy(content, variable, ft_strlen(variable) + 1);
	content[ft_strlen(variable)] = ' ';
	content[ft_strlen(variable) + 1] = 0;
	ft_strlcat(content, value, ft_strlen(value) + 1);
	to_append = node_create(content);
	if (!to_append)
		return (free(content), EXIT_FATAL);
	list_append(envp, to_append);
	return (EXIT_SUCCESS);
}