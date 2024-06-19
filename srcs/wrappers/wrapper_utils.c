/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:54:38 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/19 10:41:27 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

static size_t	list_count(t_node *head);

char	**list_cpy_to_str_arr(t_node *lst)
{
	size_t	elem_count;
	size_t	i;
	char	**self;

	if (!lst)
		return (NULL);
	elem_count = list_count(lst);
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

int	wunset(t_astnode *word, t_node **envp)
{
	t_node	*args;

	args = word->data.command.args;
	if (args)
		args = args->next;
	while (args)
	{
		if (unset(envp, args->content))
			return (EXIT_FAILURE);
		if (find_variable(envp, args->content))
			if (unset(envp, args->content))
				return (EXIT_FAILURE);
		args = args->next;
	}
	return (EXIT_SUCCESS);
}

static size_t	list_count(t_node *lst)
{
	size_t	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
