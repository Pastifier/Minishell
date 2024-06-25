/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_dump.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 18:29:53 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/25 23:45:15 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include <readline/readline.h>
#include <stdio.h>

extern volatile sig_atomic_t	g_signal;

static void	get_rid_of_garbage(t_shcontext *mshcontext, int *pipedes);
static void	catch_sigint_heredoc(int signum);

int	store_heredoc_input(t_astnode *lredir, int *pipedes,
		t_shcontext *mshcontext)
{
	char	*buffer;

	signal(SIGINT, catch_sigint_heredoc);
	close(pipedes[READ_END]);
	while (true)
	{
		buffer = readline("> ");
		if (!buffer && g_signal == SIGINT)
			break ;
		if (!buffer)
		{
			printf(HD, "msh", "warning", "here-document",
				lredir->data.redirection.filename);
			break ;
		}
		if (!ft_strncmp(buffer, lredir->data.redirection.filename, -1))
		{
			free(buffer);
			break ;
		}
		expand_env_vars(mshcontext, &buffer);
		write(pipedes[WRITE_END], buffer, ft_strlen(buffer));
		(write(pipedes[WRITE_END], "\n", 1), free(buffer));
	}
	(get_rid_of_garbage(mshcontext, pipedes), exit(EXIT_SUCCESS));
}

static void	get_rid_of_garbage(t_shcontext *mshcontext, int *pipedes)
{
	close(pipedes[WRITE_END]);
	destroy_ast(mshcontext->root);
	if (mshcontext->allocated_envp)
		str_arr_destroy(mshcontext->allocated_envp);
	list_destroy(&mshcontext->envl);
	restore_iodes(mshcontext, true);
	if (g_signal == SIGINT)
		exit(g_signal + 128);
}

static void	catch_sigint_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = SIGINT;
		write(STDIN_FILENO, "\n\r", 1);
		close(STDIN_FILENO);
	}
}
