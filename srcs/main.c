#include "parser.h"
#include "interpreter.h"
#include <readline/readline.h>
#include <readline/history.h>

int	g_signal = 0;

static bool init_envl(t_node **envl);

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
	str_arr_dup_to_list(envp, &envl);
	while (true)
	{
		ast = NULL;
		token_list = NULL;
		line = readline(prompt);
		if (line == NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		// printf("line: %s\n", line);
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
				// print_ast(ast);
				interpret(ast, envl);
				// destroy_ast(ast);
				add_history(line);
			}
		}
		// ft_putnbr_fd(*(int*)(envl->content), 1);
		// ft_putendl_fd("", 1);
		free(line);
		rl_on_new_line();
	}
	write(1, "exit\n", 5);
	list_destroy(&envl);
	clear_history();
	// destroy_mini_shell(&token_list, &ast, EXIT_SUCCESS);
}

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
