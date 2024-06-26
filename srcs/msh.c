/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:35:21 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/26 13:43:13 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include "minishell.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>

static void	processing(int parse_ret, t_astnode **ast, t_token **token_list,
				t_node **envl);
static int	unexpected_return(char **line, t_node **envl);

int	parse_line(char **line, t_astnode **ast, t_token **token_list,
		t_node **envl)
{
	int	parse_ret;

	parse_ret = init_tokenizer(*line, ast, token_list, envl);
	if (parse_ret == 4)
	{
		parse_ret = pipe_at_eol(line, envl, ast);
		destroy_mini_shell(token_list, ast, parse_ret);
		if (parse_ret == 5)
		{
			free(*line);
			*(int *)(*envl)->content = 130;
			write(STDOUT_FILENO, "\n", 1);
			return (5);
		}
		else if (parse_ret == EXIT_UNEXPECTED)
			return (unexpected_return(line, envl));
		if (!parse_ret)
			parse_ret = init_tokenizer(*line, ast, token_list, envl);
	}
	return (parse_ret);
}

static int	unexpected_return(char **line, t_node **envl)
{
	if (!OS_IS_MAC)
	{
		list_destroy(envl);
		free(*line);
		return (EXIT_SYNTAX_ERR);
	}
	else
	{
		free(*line);
		return (5);
	}
}

int	reading_line(char **line, t_node **envl, t_astnode **ast,
		t_token **token_list)
{
	int	parse_ret;

	if (*line[0] != '\0')
	{
		parse_ret = parse_line(line, ast, token_list, envl);
		if (parse_ret == 5)
			return (5);
		else if (parse_ret == EXIT_SYNTAX_ERR)
			return (parse_ret);
		(add_history(*line), free(*line));
		processing(parse_ret, ast, token_list, envl);
	}
	else
		free(*line);
	return (0);
}

static void	processing(int parse_ret, t_astnode **ast, t_token **token_list,
		t_node **envl)
{
	if (parse_ret)
	{
		destroy_mini_shell(token_list, ast, parse_ret);
		*(int *)(*envl)->content = EXIT_SYNTAX_ERR;
	}
	else
	{
		interpret(*ast, *envl);
		destroy_mini_shell(token_list, ast, 0);
	}
}
