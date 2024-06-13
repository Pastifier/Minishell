#include "parser.h"
#include "interpreter.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

int	g_signal = 0;

static bool init_envl(t_node **envl);
static bool	init_shlvl(t_node *envl);

int	main(int argc, char **argv, char **envp)
{
	char 		*prompt = "$> ";
	char  		*line;
	t_astnode	*ast;
	t_token		*token_list;
	t_node		*envl;
	int			parse_ret;

	((void)argc, (void)argv, envl = NULL);
	if (!init_envl(&envl))
		return (EXIT_FATAL);
	if (!str_arr_dup_to_list(envp, &envl))
		return (EXIT_FATAL);
	if (!init_shlvl(envl))
		return (list_destroy(&envl), EXIT_FATAL);
	while (true)
	{
		ast = NULL;
		token_list = NULL;
		line = readline(prompt);
		if (line == NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			break;
		}
		if (g_signal == 130)
			*(int*)envl->content = 130;
		if (line[0] != '\0')
		{
			parse_ret = init_tokenizer(line, &ast, &token_list, &envl);
			if (parse_ret)
			{
				printf("error value: %d\n", parse_ret); // should be in stderr
				destroy_mini_shell(&token_list, &ast, parse_ret);
			}
			else
			{
				interpret(ast, envl);
				destroy_ast(ast);
			}
			add_history(line);
		}
		free(line);
		rl_on_new_line();
	}
	write(1, "exit\n", 5);
	list_destroy(&envl);
	clear_history();
	// destroy_mini_shell(&token_list, &ast, EXIT_SUCCESS);
}

// @author Emran BinJamaan
//
// @brief 	Initialize the environment list with the first node being
//			invisible and containing the exit-code; which will be used
//			later to determine the exit status of the last-executed command.
//
// @param	envl The environment list to initialize.
//
// @warning The environment list must be freed by the caller. If the allocation fails,
//			the function will free the allocated memory and set the
//			pointer to `NULL`.
static bool init_envl(t_node **envl)
{
	*envl = malloc(sizeof(t_node));
	if (!*envl)
		return (false);
	(*envl)->content = ft_calloc(1, sizeof(int));
	if (!(*envl)->content)
		return (free(*envl), *envl = NULL, false);
	*(int*)(*envl)->content = EXIT_SUCCESS;
	(*envl)->next = NULL;
	(*envl)->prev = NULL;
	(*envl)->visible = false;
	(*envl)->is_env = false;
	return (true);
}

static bool init_shlvl(t_node *envl)
{
	t_node	*shlvl;
	char	*eql_addr;
	char	*shlvl_value_str;

	shlvl = find_variable(&envl, "SHLVL=");
	if (!shlvl)
	{
		if (bltin_export(&envl, "SHLVL=", "1"))
			return (false);
		return (true);
	}
	eql_addr = ft_strchr(shlvl->content, '=');
	shlvl_value_str = ft_itoa(ft_atoi(eql_addr + 1).value + 1);
	if (!shlvl_value_str)
		return (false);
	if (bltin_export(&envl, "SHLVL=", shlvl_value_str))
		return (free(shlvl_value_str), false);
	free(shlvl_value_str);
	return (true);
}
