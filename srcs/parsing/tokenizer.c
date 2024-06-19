/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 15:28:11 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/19 21:54:14 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

int	init_tokenizer(char *line, t_astnode **ast, t_token **token_list,
		t_node **envl)
{
	int		ret;
	t_token	*iter;

	ret = tokenize(line, token_list);
	if (ret)
		return (destroy_tokens(token_list), ret);
	ret = parse_spaces_dollars(token_list, envl);
	if (ret)
		return (destroy_tokens(token_list), ret);
	if (!*token_list)
		return (0);
	iter = *token_list;
	ret = parse(&iter, ast, envl);
	destroy_tokens(token_list);
	if (ret)
		return (ret);
	return (0);
}

int	tokenize(char *line, t_token **token_list)
{
	char			*temp;
	unsigned int	i;
	int				ret;

	temp = line;
	i = 0;
	if (!temp || !*temp)
		return (0);
	escape_special_char(temp, &i);
	if (i != 0)
	{
		ret = get_token(temp, i, token_list, TK_WORD);
		if (ret)
			return (ret);
	}
	if (temp[i])
	{
		ret = get_special_char_token(&temp[i], &i, token_list);
		if (ret)
			return (ret);
	}
	if (temp && temp[i])
		return (tokenize(&temp[i], token_list));
	return (0);
}

void	escape_special_char(char *temp, unsigned int *i)
{
	char	*special_char;

	special_char = " $|><\"'";
	if (*i != 0)
	{
		special_char = "@#$%^&*-+/={}|:<>[]\"'?~\\,.;";
		if (temp[1] == '?')
			(*i)++;
	}
	while (temp && temp[*i] && !char_in_str(temp[*i], special_char))
		(*i)++;
}

int	escape_quots(char *temp, unsigned int *i, t_token **token_list)
{
	char	c;
	int		ret;

	c = temp[*i];
	(*i)++;
	if (temp[*i] == c)
	{
		ret = get_token(&temp[0], *i + 1, token_list, TK_WORD);
		if (ret)
			return (ret);
		return ((*i)++, 0);
	}
	while (temp[*i] && temp[*i] != c)
		(*i)++;
	if (temp[*i] == '\0')
		return (3);
	if (c == '"')
		dollar_check(&temp[1], token_list, i);
	else if (c == '\'')
	{
		ret = get_token(&temp[1], *i - 1, token_list, TK_WORD);
		if (ret)
			return (ret);
	}
	return ((*i)++, 0);
}

int	dollar_check(char *str, t_token **token_list, unsigned int *i)
{
	unsigned int	j;
	unsigned int	k;
	int				ret;

	j = 0;
	while (str[j] && str[j] != '$' && str[j] != '"')
		j++;
	if (j != 0)
	{
		ret = get_token(str, j, token_list, TK_WORD);
		if (ret)
			return (1);
	}
	if (str[j] == '$')
	{
		k = 1;
		escape_special_char(&str[j], &k);
		ret = get_token(&str[j], k, token_list, TK_DOLLAR);
		if (ret)
			return (1);
		j += k;
	}
	if (str[j] != '"')
		return (dollar_check(&str[j], token_list, i));
	return (0);
}
