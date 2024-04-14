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
	// char 		*prompt = "$> ";
	// char  		*line;
	// t_astnode	*ast;
	t_node		*envl;

	((void)argc, (void)argv);
	envl = NULL;
	str_arr_dup_to_list(envp, &envl);
	// while (1)
	// {
	// 	line = readline(prompt);
	// 	if (line == NULL)
	// 		break;
	// 	if (line[0] != '\0')
	// 	{
	// 		add_history(line) ;
	// 		ast = tokenize(line);
	// 		if (ast)
	// 		{
	// 			interpret(ast, envl);
	// 			destroy_ast(ast);
	// 		}
	// 	}
	// 	free(line);
	// }
	t_astnode word, word2, word3, piper, piper2;
	piper = (t_astnode)
	{
		.type = TK_PIPE,
		.parent = &piper2,
		.left = &word,
		.right = &word2
	};
	piper2 = (t_astnode)
	{
		.type = TK_PIPE,
		.parent = NULL,
		.left = &piper,
		.right = &word3
	};
	word = (t_astnode)
	{
		.type = TK_WORD,
		.parent = &piper,
		.left = NULL,
		.right = NULL,
		.data.command.args = (char*[3]){"/bin/cat", "Makefile", NULL},
		.data.command.thereispipe = false
	};
	word2 = (t_astnode)
	{
		.type = TK_WORD,
		.parent = &piper,
		.left = NULL,
		.right = NULL,
		.data.command.args = (char*[3]){"grep", "s", NULL},
		.data.command.thereispipe = true
	};
	word3 = (t_astnode)
	{
		.type = TK_WORD,
		.parent = &piper2,
		.left = NULL,
		.right = NULL,
		.data.command.args = (char*[3]){"wc", "-l", NULL},
		.data.command.thereispipe = false
	};
	interpret(&piper, envl);
	return (0);
}
