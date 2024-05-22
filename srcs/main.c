#include "minishell.h"
#include "parser.h"
#include "interpreter.h"
#include <readline/readline.h>
#include <readline/history.h>

int	g_signal = 0;

// minishell entry point
/*
main function calls readline and call the tokenizer and the  parser to execute commands
*/
int	main(int argc, char **argv, char **envp)
{
	char 		*prompt = "$> ";
	char  		*line;
	t_astnode	*ast;
	t_node		*envl;
	int parse_ret;

	((void)argc, (void)argv, envl = NULL);
	str_arr_dup_to_list(envp, &envl);
	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
			break;
		ast = NULL;
		if (line[0] != '\0')
		{
			parse_ret = init_tokenizer(line, &ast);
			if (parse_ret)
			{
				printf("%i\n", parse_ret);
			}
			// 	// print_ast(ast);
			// 	interpret(ast, envl);
				// destroy_ast(ast);
				add_history(line);
			// }
		}
		free(line);
	}
	return (0);
}
