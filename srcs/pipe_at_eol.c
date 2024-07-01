/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_at_eol.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:29:13 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/26 14:25:27 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include "parser.h"
#include <readline/readline.h>

extern volatile sig_atomic_t	g_signal;

int	_wpipe_at_eol_parent(int *fd, char **line, int *ret, pid_t pid);

static void	catch_sigint(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = 130;
		write(STDIN_FILENO, "\n\r", 1);
		close(STDIN_FILENO);
	}
}

int	pipe_at_eol(char **line, t_node **envl, t_astnode **ast)
{
	int		ret;
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		close(fd[0]);
		free(*line);
		*line = NULL;
		ret = pipe_at_eol_child(line, envl, fd, ast);
		exit(0);
	}
	else
		return (_wpipe_at_eol_parent(fd, line, &ret, pid));
	return (ret);
}

int	_wpipe_at_eol_parent(int *fd, char **line, int *ret, pid_t pid)
{
	signal(SIGINT, SIG_IGN);
	waitpid(pid, ret, 0);
	if (WEXITSTATUS(*ret) == 130)
		return (close(fd[0]), close(fd[1]), 5);
	if (WIFSIGNALED(*ret))
	{
		if (WTERMSIG(*ret) == SIGINT)
			return (close(fd[0]), close(fd[1]), 5);
	}
	if (WEXITSTATUS(*ret) == 0)
		return (pipe_at_eol_parent(line, fd));
	else if (WEXITSTATUS(*ret) == EXIT_UNEXPECTED)
		return (close(fd[0]), close(fd[1]), EXIT_UNEXPECTED);
	return (close(fd[0]), close(fd[1]), WEXITSTATUS(*ret));
}

int	pipe_at_eol_child(char **line, t_node **envl, int fd[2], t_astnode **ast)
{
	destroy_mini_shell(NULL, ast, 0);
	signal(SIGINT, catch_sigint);
	while (true && g_signal != 130)
	{
		*line = readline("> ");
		if (!*line && g_signal == 130)
			break ;
		if (*line == NULL)
		{
			ft_putendl_fd("msh: syntax error: unexpected EOF", 2);
			(close(fd[0]), close(fd[1]));
			list_destroy(envl);
			exit(EXIT_UNEXPECTED);
		}
		if ((*line)[0] != '\0')
			__pipe_at_eol_confirm(line, envl, fd);
		free(*line);
	}
	return (__end_child_process(envl, fd));
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
	*line = ft_strjoin(temp, new_line);
	free(temp);
	free(new_line);
	if (!*line)
		return (EXIT_FAILURE);
	return (0);
}
