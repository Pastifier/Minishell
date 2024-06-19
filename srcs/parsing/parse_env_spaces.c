/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env_spaces.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 17:41:09 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/19 20:36:15 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

static	int	check_env(t_token **token_list, t_node **envl);
static	int	parse_env(t_token **token_list, t_node **envl);
static	int	get_exit_status(t_token **token_list, t_node **envl);
static	int	join_env(t_token **token_list);

int	parse_spaces_dollars(t_token **token_list, t_node **envl)
{
	t_token	**iter;
	int		ret;

	ret = check_env(token_list, envl);
	if (ret)
		return (ret);
	if (*token_list == NULL)
		return (0);
	iter = token_list;
	while (*iter)
	{
		if ((*iter)->token_type == TK_SPACE)
			remove_token(&(*iter));
		else
			iter = &(*iter)->next;
	}
	return (0);
}

static	int	check_env(t_token **token_list, t_node **envl)
{
	t_token	**iter;
	int		ret;

	iter = token_list;
	while (*iter)
	{
		if ((*iter)->token_type == TK_DOLLAR)
		{
			ret = parse_env(&(*iter), envl);
			if (ret)
				return (ret);
		}
		else
			iter = &(*iter)->next;
	}
	return (0);
}

static	int	parse_env(t_token **token_list, t_node **envl)
{
	t_node	*iter;
	char	*eql_addr;
	char	*env_value;

	iter = *envl;
	(*token_list)->token_type = TK_WORD;
	if (!(*token_list)->value[1])
		return (join_env(token_list));
	env_value = &(*token_list)->value[1];
	if ((*token_list)->value[1] == '?')
		return (get_exit_status(token_list, envl));
	while (iter)
	{
		if (!ft_strncmp(env_value, iter->content, ft_strlen(env_value)))
		{
			free((*token_list)->value);
			eql_addr = ft_strchr(iter->content, '=');
			(*token_list)->value = ft_strdup(eql_addr + 1);
			if (!(*token_list)->value)
				return (1);
			return (join_env(token_list));
		}
		iter = iter->next;
	}
	return ((*token_list)->token_type = TK_DUMMY, 0);
}

static	int	get_exit_status(t_token **token_list, t_node **envl)
{
	char	*env_value;
	char	*eql_addr;

	env_value = &(*token_list)->value[1];
	if ((*token_list)->value[1] == '?')
	{
		env_value = (*token_list)->value;
		(*token_list)->value = ft_itoa(*(int *)(*envl)->content);
		if (!(*token_list)->value)
			return (free(env_value), 1);
		if (env_value[2])
		{
			eql_addr = (*token_list)->value;
			(*token_list)->value = ft_strjoin((*token_list)->value, env_value
					+ 2);
			free(eql_addr);
			if (!(*token_list)->value)
				return (free(env_value), 1);
		}
		return (free(env_value), join_env(token_list));
	}
	return (0);
}

static	int	join_env(t_token **token_list)
{
	char	*content;
	t_token	*iter;

	iter = *token_list;
	if ((*token_list)->prev && (*token_list)->prev->token_type == TK_WORD)
	{
		content = iter->prev->value;
		iter->prev->value = ft_strjoin(content, iter->value);
		if (!iter->prev->value)
			return (1);
		free(content);
		iter = iter->prev;
		remove_token(token_list);
	}
	while (iter->next && iter->next->token_type == TK_WORD)
	{
		content = iter->value;
		iter->value = ft_strjoin(content, iter->next->value);
		if (!iter->value)
			return (1);
		free(content);
		remove_token(&iter->next);
	}
	return (0);
}
