/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 17:16:10 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/19 21:43:49 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

static int	join_word(t_token **token_list, char **new, t_token_type type);

int	get_token(char *temp, unsigned int i, t_token **token_list,
		t_token_type type)
{
	char	*new;
	t_token	*new_token;
	int		ret;

	new = ft_substr(temp, 0, i);
	if (new == NULL)
		return (1);
	if (ft_strncmp(new, "\"\"", 2) == 0 || ft_strncmp(new, "''", 2) == 0)
	{
		free(new);
		new = ft_strdup("");
		if (new == NULL)
			return (1);
	}
	ret = join_word(token_list, &new, type);
	if (ret || !new)
		return (ret);
	new_token = token_create(new, type);
	if (new_token == NULL)
		return (free(new), 1);
	token_list_append(token_list, new_token);
	return (0);
}

static int	join_word(t_token **token_list, char **new, t_token_type type)
{
	t_token		*last_token;
	char		*last_value;

	if (token_list && (*token_list))
	{
		last_token = token_list_last(token_list);
		if ((type == TK_WORD && (last_token)->token_type == TK_WORD)
			|| (type == TK_SPACE && (last_token)->token_type == TK_SPACE))
		{
			last_value = (last_token)->value;
			(last_token)->value = ft_strjoin(last_value, *new);
			if (!(last_token)->value)
				return (free(*new), free(last_value), 1);
			return (free(*new), *new = NULL, free(last_value), 0);
		}
	}
	return (0);
}

int	get_special_char_token(char *temp, unsigned int *i, t_token **token_list)
{
	unsigned int	j;

	j = 0;
	if (*temp == '"' || *temp == '\'')
	{
		if (escape_quots(temp, &j, token_list))
			return (3);
		return (*i += j, 0);
	}
	else if (*temp == '$')
	{
		j = 1;
		escape_special_char(temp, &j);
		return (*i += j, get_token(temp, j, token_list, TK_DOLLAR));
	}
	else if (*(temp + 1) && *temp == *(temp + 1))
		return (*i += 2, get_token(temp, 2, token_list,
				get_token_type(temp, 2)));
	else
		return ((*i)++, get_token(temp, 1, token_list,
				get_token_type(temp, 1)));
}
