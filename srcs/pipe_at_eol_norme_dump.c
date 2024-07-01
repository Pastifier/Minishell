/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_at_eol_norme_dump.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:38:22 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/26 14:05:28 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include "parser.h"

extern volatile sig_atomic_t	g_signal;

void	__pipe_at_eol_confirm(char **line, t_node **envl, int *fd)
{
	int			ret;
	t_astnode	*new_ast;
	t_token		*token_list;

	new_ast = NULL;
	token_list = NULL;
	ret = init_tokenizer(*line, &new_ast, &token_list, envl);
	destroy_mini_shell(&token_list, &new_ast, ret);
	if (ret && ret != 4)
	{
		(close(fd[0]), close(fd[1]));
		free(*line);
		list_destroy(envl);
		exit(ret);
	}
	write(fd[1], " ", 1);
	write(fd[1], *line, ft_strlen(*line));
	if (!ret)
	{
		(close(fd[0]), close(fd[1]));
		list_destroy(envl);
		free(*line);
		exit(EXIT_SUCCESS);
	}
}

int	__end_child_process(t_node **envl, int *fd)
{
	list_destroy(envl);
	if (g_signal == 130)
	{
		(close(fd[0]), close(fd[1]));
		exit(130);
	}
	return (exit(EXIT_SUCCESS), EXIT_SUCCESS);
}
