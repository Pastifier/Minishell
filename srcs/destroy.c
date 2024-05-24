# include "minishell.h"

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

    if (!tokens || !*tokens)
        return ;
    while (tokens && (*tokens)->next)
    {
        temp = *tokens;
        *tokens = (*tokens)->next;
        free(temp->value);
        free(temp);
    }
}

void destroy_ast(t_astnode *node)
{
    if (!node)
        return ;
    if (node->type == TK_BUILTIN)
        destroy_str_arr(node->data.builtin.args);
    if (node->type == TK_WORD)
        list_destroy(&node->data.command.args);
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
    destroy_tokens(token);
    destroy_ast(*node);
    exit(1);
}

void exit_faliure(char *msg)
{
    write(2, msg, ft_strlen(msg));
    exit(1);
}
