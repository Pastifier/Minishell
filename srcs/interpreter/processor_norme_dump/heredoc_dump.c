/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_dump.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 18:29:53 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/19 21:18:04 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include <readline/readline.h>
#include <stdio.h>

static void	get_rid_of_garbage(t_shcontext *mshcontext);

int	store_heredoc_input(t_astnode *lredir, int *pipedes,
		t_shcontext *mshcontext)
{
	char	*buffer;

	signal(SIGINT, SIG_DFL);
	close(pipedes[READ_END]);
	while (true)
	{
		buffer = readline("> ");
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
		write(pipedes[WRITE_END], buffer, ft_strlen(buffer));
		write(pipedes[WRITE_END], "\n", 1);
		free(buffer);
	}
	close(pipedes[WRITE_END]);
	get_rid_of_garbage(mshcontext);
	exit(EXIT_SUCCESS);
}

static void	get_rid_of_garbage(t_shcontext *mshcontext)
{
	destroy_ast(mshcontext->root);
	if (mshcontext->allocated_envp)
		str_arr_destroy(mshcontext->allocated_envp);
	list_destroy(&mshcontext->envl);
	restore_iodes(mshcontext, true);
}
