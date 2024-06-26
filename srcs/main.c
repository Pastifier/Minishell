/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 14:23:18 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/26 14:26:22 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>

volatile sig_atomic_t	g_signal = 0;

static void	end_shell(t_node **envl)
{
	int	exit_code;

	exit_code = *(int *)(*envl)->content;
	write(1, "exit\n", 5);
	list_destroy(envl);
	clear_history();
	exit(exit_code);
}
void					signal_handler(int signum);

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_astnode	*ast;
	t_token		*token_list;
	t_node		*envl;
	int			parse_ret;

	((void)argc, (void)argv, envl = NULL);
	signal(SIGQUIT, SIG_IGN);
	if (init_msh_env(&envl, envp))
		return (EXIT_FATAL);
	while (true)
	{
		line = clean_start(&ast, &token_list, signal_handler);
		if (line == NULL)
			break ;
		parse_ret = reading_line(&line, &envl, &ast, &token_list);
		if (parse_ret == 5)
			continue ;
		else if (parse_ret == EXIT_SYNTAX_ERR)
			return (parse_ret);
		rl_on_new_line();
	}
	end_shell(&envl);
}

// @author Emran BinJamaan
//
// @brief 	Initialize the environment list with the first node being
//			invisible and containing the exit-code; which will be used
//			later to determine the exit status of the last-executed command.
//
// @param	envl The environment list to initialize.
//
// @warning The environment list must be freed by the caller.
//			If the allocation fails, the function will free
//			the allocated memory and set the pointer to `NULL`.
bool	init_envl(t_node **envl)
{
	t_node	*to_append;

	to_append = node_create("");
	if (!to_append)
		return (false);
	to_append->visible = false;
	free(to_append->content);
	to_append->content = ft_calloc(1, sizeof(int));
	if (!to_append->content)
		return (node_destroy(to_append), false);
	*(int *)to_append->content = EXIT_SUCCESS;
	list_append(envl, to_append);
	return (true);
}

bool	init_shlvl(t_node *envl)
{
	t_node	*shlvl;
	char	*eql_addr;
	char	*shlvl_value_str;

	shlvl = find_variable(&envl, "SHLVL");
	if (!shlvl)
	{
		if (bltin_export(&envl, "SHLVL", "1"))
			return (false);
		return (true);
	}
	eql_addr = ft_strchr(shlvl->content, '=');
	if (ft_atoi(eql_addr + 1).value < 0)
	{
		if (bltin_export(&envl, "SHLVL", "0"))
			return (false);
		return (true);
	}
	shlvl_value_str = ft_itoa(ft_atoi(eql_addr + 1).value + 1);
	if (!shlvl_value_str)
		return (false);
	if (bltin_export(&envl, "SHLVL", shlvl_value_str))
		return (free(shlvl_value_str), false);
	free(shlvl_value_str);
	return (true);
}

void	signal_handler(int signum)
{
	g_signal = signum;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
