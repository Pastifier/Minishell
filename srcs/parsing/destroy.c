# include "../include/minishell.h"

void destory_str_arr(char **str_arr)
{
    int i;

    i = 0;
    while (str_arr[i])
    {
        free(str_arr[i]);
        i++;
    }
    free(str_arr);
}

void destory_tokens(t_token *tokens)
{
    t_token *temp;

    while (tokens)
    {
        temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
}

void destroy_ast(t_astnode *node)
{
    t_astnode *temp;

    if (!node)
        return ;
    if (node->type == TK_BUILTIN)
        destory_args(node->data.builtin.args);
    if (node->type == TK_EXEC)
        destory_args(node->data.command.args);
    else if (node->type == TK_PIPE)
    {
        destroy_ast(node->left);
        destroy_ast(node->right);
    }
    else if (node->type == TK_LREDIR || node->type == TK_RREDIR)
    {
        free(node->data.redirection.filename);
        destroy_ast(node->left);
    }
}

void destroy_parser(t_token **token, t_astnode **node)
{
    destory_tokens(*token);
    destroy_ast(*node);
    exit(1);
}