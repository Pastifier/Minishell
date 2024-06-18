/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_dump.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 18:29:53 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/18 18:45:29 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include <readline/readline.h>
#include <stdio.h>

int	store_heredoc_input(t_astnode *lredir, int *pipedes)
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
	exit(EXIT_SUCCESS);
}
