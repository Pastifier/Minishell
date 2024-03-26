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
void print_ast(t_astnode *ast)
{
	if (ast == NULL)
		return ;
	if (ast->type == TK_COMMAND)
	{
		printf("command: %s\n", ast->data.command.args[0]);
	}
	else if (ast->type == TK_PIPE)
	{
		printf("pipe\n");
		print_ast(ast->left);
		print_ast(ast->right);
	}
	else if (ast->type == TK_LREDIR)
	{
		printf("left redirect\n");
		print_ast(ast->left);
		print_ast(ast->right);
	}
	else if (ast->type == TK_RREDIR)
	{
		printf("right redirect\n");
		print_ast(ast->left);
		print_ast(ast->right);
	}
	else if (ast->type == TK_D_QUOTE)
	{
		printf("double quote\n");
		print_ast(ast->left);
	}
	else if (ast->type == TK_S_QUOTE)
	{
		printf("single quote\n");
		print_ast(ast->left);
	}
	else if (ast->type == TK_WORD)
	{
		printf("word: %s\n", ast->data.command.args[0]);
	}
	else if (ast->type == TK_BUILTIN)
	{
		printf("builtin: %s\n", ast->data.builtin.args[0]);
	}
	else if (ast->type == TK_EXEC)
	{
		printf("exec: %s\n", ast->data.command.args[0]);
	}
}

