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
  t_astnode root, command, redirection1, redirection2;
  command = (t_astnode){0};
  command = (t_astnode)
    {
      .type = TK_WORD,
      .parent = NULL,
      .left = &redirection1,
      .right = NULL,
      .data.command.args = malloc(sizeof(t_node)),
      .data.command.args->content = "ls",
      .data.command.args->next = NULL,
      .data.command.args->prev = NULL
    };
  redirection1 = (t_astnode){0};
  redirection1 = (t_astnode)
    {
      .type = TK_RREDIR,
      .parent = &command,
      .left = &redirection2,
      .right = NULL,
      .data.redirection.filename = "falafel",
    };

  redirection2 = (t_astnode){0};
  redirection2 = (t_astnode)
    {
      .type = TK_RREDIR,
      .parent = &redirection1,
      .left = NULL,
      .right = NULL,
      .data.redirection.filename = "falafel2",
    };
  (void)root;
  interpret(&command, envl);
  return (0);
}
