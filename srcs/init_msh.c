/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_msh.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 10:32:45 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/26 12:39:36 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include "minishell.h"
#include "parser.h"
#include <readline/readline.h>
#include <readline/history.h>

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

void	processing(int parse_ret, t_astnode **ast, t_token **token_list,
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

char	*clean_start(t_astnode **ast, t_token **token_list,
            void (*signal_handler)(int))
{
    char    *line;

    signal(SIGINT, signal_handler);
    *ast = NULL;
    *token_list = NULL;
    line = readline("$> ");
    if (line == NULL)
        write(STDOUT_FILENO, "\n", 1);
    return (line);
}

int parse_line(char **line, t_astnode **ast, t_token **token_list, t_node **envl)
{
    int parse_ret;

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
        if (!parse_ret)
            parse_ret = init_tokenizer(*line, ast, token_list, envl);
    }
    return (parse_ret);
}
    
