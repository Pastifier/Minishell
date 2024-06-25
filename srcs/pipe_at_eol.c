/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_at_eol.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:29:13 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/25 18:08:33 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include "parser.h"
#include <readline/readline.h>

int pipe_at_eol(char **line, t_node **envl, t_astnode **ast)
{
	int 	ret;
	pid_t	pid;
	int fd[2];

	if (pipe(fd) == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		close(fd[0]);
		ret = pipe_at_eol_child(line, envl, fd, ast);
		exit(0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &ret, 0);
		if (WIFSIGNALED(ret))
		{
			if (WTERMSIG(ret) == SIGINT)
				return (close(fd[0]), close(fd[1]), 5);
		}
		if (WEXITSTATUS(ret) == 0)
			return (pipe_at_eol_parent(line, fd));
		else if (WEXITSTATUS(ret) == EXIT_UNEXPECTED && !OS_IS_MAC)
			return (close(fd[0]), close(fd[1]), EXIT_UNEXPECTED);
		return (close(fd[0]), close(fd[1]), 4);
	}
	return (ret);
}

int pipe_at_eol_child(char **line, t_node **envl, int fd[2], t_astnode **ast)
{
	int			ret;
	t_astnode	*new_ast;
	t_token		*token_list;

	new_ast = NULL;
	token_list = NULL;
	free(*line);
	destroy_mini_shell(NULL, ast, 0);
	signal(SIGINT, SIG_DFL);
	while (true)
	{
		*line = readline("> ");
		if (*line == NULL)
		{
			ft_putendl_fd("msh: syntax error: unexpected EOF", 2);
			exit(EXIT_UNEXPECTED);
		}
		if ((*line)[0] != '\0')
		{
			ret = init_tokenizer(*line, &new_ast, &token_list, envl);
			destroy_mini_shell(&token_list, &new_ast, ret);
			if (ret && ret != 4)
				exit(ret);		
			write(fd[1], " ", 1);
			write(fd[1], *line, ft_strlen(*line));
			if (!ret)
			{
				(close(fd[0]), close(fd[1]));
				list_destroy(envl);
				exit(EXIT_SUCCESS);
			}
		}
		free(*line);
	}
	list_destroy(envl);
	exit(EXIT_SUCCESS);
}

int	pipe_at_eol_parent(char **line, int fd[2])
{
	char	*temp;
	char	*new_line;

	close(fd[1]);
	temp = *line;
	new_line = (get_next_line(fd[0]).line);
	if (!new_line)
		return (close(fd[0]), 1);
	close(fd[0]);
	if (!*line)
		return (free(temp), 1);
	*line = ft_strjoin(temp, new_line);
	free(temp);
	free(new_line);
	if (!*line)
		return (EXIT_FAILURE);
	return (0);
}
