# include "minishell.h"
# include <stdio.h>

void print_ast(t_astnode *ast)
{
	if (ast == NULL)
		return ;
	printf("type: %d\n", ast->type);
	if (ast->type == TK_WORD)
	{
		print_array(ast->data.command.args);
	}
	else if (ast->type == TK_PIPE)
	{
		printf("pipe\n");
        printf("left:");
		print_ast(ast->left);
        printf("right:");
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
	else if (ast->type == TK_DBLQT)
	{
		printf("double quote\n");
		print_ast(ast->left);
	}
	else if (ast->type == TK_SGLQT)
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
}


// print_array function
void print_array(char **array)
{
	int i;

	i = 0;
    printf("array: ");
	while (array[i])
	{
		printf("%s\n", array[i]);
		i++;
	}
}

void print_tokens(t_token **token)
{
    t_token *iter;

    iter = *token;
    while (iter)
    {
        printf("token: %s type: %u\n", iter->value, iter->token_type);
        iter = iter->next;
    }
}
