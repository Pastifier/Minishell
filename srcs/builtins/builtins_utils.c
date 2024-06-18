/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 06:41:42 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/18 10:31:03 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_iswhitespace(int c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

// @author Emran BinJamaan
// @warning This function assumes that `string` starts
// with the '$' character.
char	*extract_variable(char *string)
{
	size_t	count;
	char	*self;
	char	*no_var;

	count = 0;
	if (*(string + 1))
		++string;
	while (string[count] && !ft_iswhitespace(string[count]))
		++count;
	if (count == 0)
		return (NULL);
	self = ft_substr(string, 0, count);
	return (self);
}
