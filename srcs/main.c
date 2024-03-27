#include "minishell.h"

// minishell entry point
/*
main function calls readline and call the tokenizer and the  parser to execute commands
*/
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char *prompt = "$> ";
	char  *line;
	t_astnode *ast;

	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
			break;
		if (line[0] != '\0')
		{
			add_history(line) ;
			ast = tokenize(line);
			if (ast != NULL)
				print_ast(ast);
		}
		free(line);
	}
	return (0);
}

// print_ast function


