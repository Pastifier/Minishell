# include "minishell.h"
#include <stdio.h>

void destroy_str_arr(char **str_arr)
{
    int i;

    if (!str_arr || !*str_arr)
        return ;
    i = 0;
    while (str_arr[i])
    {
        free(str_arr[i]);
        i++;
    }
    free(str_arr);
}

void destroy_tokens(t_token **tokens)
{
	t_token *temp;
	t_token **original; // Emran

	if (!tokens || !*tokens)
		return ;
	original = &(*tokens); // Emran
	while (*tokens)
	{
		temp = *tokens;
		*tokens = (*tokens)->next;
		free(temp->value);
		free(temp);
	}
	*original = NULL; // Emran
}

void destroy_ast(t_astnode *node)
{
	if (!node)
		return ;
	destroy_ast(node->left);
	destroy_ast(node->right);
	if (node->type == TK_WORD)
	{
		free(node->data.command.cmd);
		list_destroy(&node->data.command.args);
		free(node);
	}
	else if (node->type == TK_LREDIR || node->type == TK_RREDIR)
	{
		free(node->data.redirection.filename);
		free(node);
	}
	else
		free(node);
}

void destroy_mini_shell(t_token **token, t_astnode **node, int exit_status)
{
    (void)token;
    destroy_ast(*node);
    show_error(exit_status);
	*node = NULL;
}

void show_error(int exit_status)
{
    char *msg;

    if (exit_status == 0)
        exit(0);
    if (exit_status == 1)
        msg = "Error: malloc failed\n";
    else if (exit_status == 2)
        msg = "Error: syntax error near unexpected token `|'\n";
    else if (exit_status == 3)
        msg = "Error: unclosed quotes\n";
    else if (exit_status == 4)
        msg = "Error: command not found\n";
    else if (exit_status == 5)
        msg = "Error: fork failed\n";
    else if (exit_status == 6)
        msg = "Error: execve failed\n";
    write(2, msg, ft_strlen(msg));
}
