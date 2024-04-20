# include "minishell.h"
# include <stdio.h>

void print_ast(t_astnode *ast)
{
	if (ast == NULL)
	{
		printf("ast is null\n");
		return ;
	}
	printf("type: %d\n", ast->type);
	if (ast->type == TK_WORD)
	{
		printf("cmd: %s\n", ast->data.command.cmd);
		print_list(&ast->data.command.args);
		printf("Left: \n");
		print_ast(ast->left);
		printf("Right: \n");
		print_ast(ast->right);
	}
	else if (ast->type == TK_PIPE)
	{
		printf("pipe\n");
        printf("left:\n");
		print_ast(ast->left);
        printf("right:\n");
		print_ast(ast->right);
	}
	else if (ast->type == TK_LREDIR)
	{
		printf("left ledirect\n");
		printf("file name: %s\n", ast->data.redirection.filename);
		printf("left:");
		print_ast(ast->left);
		printf("right: ");
		print_ast(ast->right);
	}
	else if (ast->type == TK_RREDIR)
	{
		printf("right redirect\n");
		printf("file name: %s\n", ast->data.redirection.filename);
		printf("left: ");
		print_ast(ast->left);
		printf("right: ");
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
	// else if (ast->type == TK_WORD)
	// {
	// 	printf("word: %s\n", ast->data.command.args[0]);
	// }
	// else if (ast->type == TK_BUILTIN)
	// {
	// 	printf("builtin: %s\n", ast->data.builtin.args[0]);
	// }
}


// print_array function
void print_array(char **array)
{
	int i;

	i = 0;
    printf("array: ");
	while (array[i])
	{
		printf(" - %s", array[i]);
		i++;
	}
	printf("\n");
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

void print_list(t_node **head)
{
	t_node *iter;

	iter = *head;
	while (iter)
	{
		printf("node: %s\n", (char *)iter->content);
		iter = iter->next;
	}
}
