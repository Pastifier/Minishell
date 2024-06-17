/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:54:38 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/17 20:30:49 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

char	**list_cpy_to_str_arr(t_node *lst)
{
	size_t	elem_count;
	t_node	*dummy;
	size_t	i;
	char	**self;

	if (!lst)
		return (NULL);
	dummy = lst;
	elem_count = 0;
	while (dummy)
		(elem_count++, dummy = dummy->next);
	if (elem_count == 0)
		return (NULL);
	self = ft_calloc((elem_count + 1), sizeof(char *));
	i = 0;
	while (i < elem_count)
	{
		self[i] = ft_strdup(lst->content);
		if (!self[i])
				return (str_arr_destroy(self), NULL);
		lst = lst->next;
		i++;
	}
	return (self);
}

void	str_arr_destroy(char **strarr)
{
	char	**dummy;

	if (!strarr)
		return ;
	dummy = strarr;
	while (*dummy)
	{
		if (*dummy)
			free(*dummy);
		dummy++;
	}
	free(strarr);
}

int		wexport(t_astnode *word, t_node **envp, const char *variable,
	const char *value)
{
	t_node	*args;

	args = word->data.command.args;
	while (args)
	{
		if (*(char*)args->content == '=')
		{
			ft_putstr_fd("msh: export: `", STDERR_FILENO);
			ft_putstr_fd((char *)variable, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		if (parse_export(variable) == 1)
		{
			ft_putstr_fd("msh: export: `", STDERR_FILENO);
			ft_putstr_fd((char *)variable, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		if (bltin_export(envp, variable, value))
			return (EXIT_FAILURE);
		args = args->next;
	}
	return (EXIT_SUCCESS);
}
