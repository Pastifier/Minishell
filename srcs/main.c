#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

int	g_signal = 0;

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

	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
			break;
		if (line[0] != '\0')
		{
			add_history(line) ;
			tokenize(line);
		}
		free(line);
	}
	return (0);
}
