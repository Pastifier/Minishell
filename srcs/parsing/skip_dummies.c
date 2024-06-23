/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_dummies.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 09:40:43 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/23 09:40:48 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static	int		skip_dummy_helper(t_token ***iter);

int	skip_dummies(t_token **token_list)
{
	t_token	**iter;

	iter = token_list;
	while (*iter)
	{
		if ((*iter)->token_type == TK_WORD && (*iter)->next
			&& (*iter)->next->token_type == TK_DUMMY)
		{
			if (skip_dummy_helper(&iter))
				return (1);
		}
		else
			iter = &(*iter)->next;
	}
	return (0);
}

static	int	skip_dummy_helper(t_token ***iter)
{
	t_token		*this_token;
	char		*temp;

	this_token = **iter;
	*iter = &(**iter)->next;
	while (**iter && (**iter)->token_type == TK_DUMMY)
		*iter = &(**iter)->next;
	while (**iter && ((**iter)->token_type == TK_WORD
			|| (**iter)->token_type == TK_DUMMY))
	{
		if ((**iter)->token_type == TK_DUMMY)
			*iter = &(**iter)->next;
		else
		{
			temp = this_token->value;
			this_token->value = ft_strjoin(this_token->value, (**iter)->value);
			free(temp);
			if (!this_token->value)
				return (1);
			remove_token(&(**iter));
		}
	}
	return (0);
}
