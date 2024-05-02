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
  //char 		*prompt = "$> ";
  //char		*line;
  //t_astnode	*ast;
	t_node		*envl;

	((void)argc, (void)argv, envl = NULL);
	str_arr_dup_to_list(envp, &envl);
  //while (1)
  //{
  //	line = readline(prompt);
  //	if (line == NULL)
  //		break;
  //	if (line[0] != '\0')
  //	{
  //		ast = init_tokenizer(line);
  //		if (ast)
  //		{
  //			// print_ast(ast);
  //			interpret(ast, envl);
  //			// destroy_ast(ast);
  //			add_history(line);
  //		}
  //	}
  //	free(line);
  //	rl_on_new_line();
  //}
	t_astnode root, command, command2, redirection1, redirection2;
	command = (t_astnode){0};
	t_node *args = malloc(sizeof(t_node));
	t_node *args2 = malloc(sizeof(t_node));
	args->content = "cat";
	args->next = NULL;
	args->prev = NULL;
	args2->content = "cat";
	args2->next = NULL;
	args2->prev = NULL;
	root = (t_astnode){0};
	root = (t_astnode)
	{
		.type = TK_PIPE,
		.parent = NULL,
		.left = &command,
		.right = &command2
	};
	command = (t_astnode)
	{
	  .type = TK_WORD,
	  .parent = &root,
	  .left = NULL,
	  .right = &redirection1,
	  .data.command.args = args
	};
	command2 = (t_astnode)
	{
		.type = TK_WORD,
		.parent = &root,
		.left = NULL,
		.right = NULL,
		.data.command.args = args2
	};

	redirection1 = (t_astnode){0};
	redirection1 = (t_astnode)
	{
	  .type = TK_LREDIR,
	  .parent = &command,
	  .left = NULL,
	  .right = &redirection2,
	  .data.redirection.filename = "falafel",
	};

  	redirection2 = (t_astnode){0};
  	redirection2 = (t_astnode)
	{
	  .type = TK_LREDIR,
	  .parent = &redirection1,
	  .left = NULL,
	  .right = NULL,
	  .data.redirection.filename = "falafel2",
	};
	interpret(&command, envl);
	return (0);
}
