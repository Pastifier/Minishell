/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:54:38 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/23 12:44:51 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**list_cpy_to_str_arr(t_node *lst)
{
	size_t	elem_count;
	t_node	*dummy;
	char	**self;

	if (!lst)
		return (NULL);
	dummy = lst;
	elem_count = 0;
	if (!dummy->next)
		elem_count++;
	while (dummy->next)
	{
		elem_count++;
		dummy = dummy->next;
		if (!dummy->next)
			elem_count++;
	}
	self = ft_calloc((elem_count + 1), sizeof(char *));
	while (dummy)
	{
		self[--elem_count] = ft_strdup(dummy->content);
		// guard ft_strdup somehow
		dummy = dummy->prev;
	}
	return (self);
}

void	str_arr_destroy(char **strarr)
{
	char	**dummy;

	dummy = strarr;
	while (*dummy)
		free(*dummy++);
	free(strarr);
}
