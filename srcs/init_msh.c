/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_msh.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 10:32:45 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/26 13:42:15 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include "minishell.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>

extern volatile sig_atomic_t	g_signal;

int	init_msh_env(t_node **envl, char **envp)
{
	if (!init_envl(envl))
		return (EXIT_FATAL);
	if (!str_arr_dup_to_list(envp, envl))
		return (EXIT_FATAL);
	if (!init_shlvl(*envl))
		return (list_destroy(envl), EXIT_FATAL);
	return (EXIT_SUCCESS);
}

char	*clean_start(t_astnode **ast, t_token **token_list,
		void (*signal_handler)(int))
{
	char	*line;

	signal(SIGINT, signal_handler);
	*ast = NULL;
	*token_list = NULL;
	line = readline("$> ");
	if (line == NULL)
		write(STDOUT_FILENO, "\n", 1);
	return (line);
}
