#include "minishell.h"
#include "parser.h"

char **get_command_args(t_token **token)
{
    char **args;
    t_token *temp;
    int i;

    i = 0;
    temp = *token;
    while (temp && temp->token_type == TK_WORD)
    {
        i++;
        temp = temp->next;
    }
	args = ft_calloc(i + 1, sizeof(char *));
    if (!args)
        return (NULL);
    temp = *token;
    i = 0;
    while (temp && temp->token_type == TK_WORD)
    {
        args[i] = ft_strdup(temp->value);
        if (!args[i])
            return (destroy_str_arr(args), NULL);
        i++;
        temp = temp->next;
    }
    args[i] = NULL;
    (*token)->next = temp;
    return (args);
}

char *ft_str_toupper(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        str[i] = ft_toupper(str[i]);
        i++;
    }
    return (str);
}
    
void add_redir_node(t_astnode **node, t_astnode *new_node)
{
    t_astnode *iter;

    if (!(*node))
    {
        (*node) = new_node;
        new_node->parent = NULL;
    }
    else
    {
        iter = *node;
        while (iter && iter->right)
            iter = iter->right;
        new_node->parent = iter;
        iter->right = new_node;
    }
}

void set_redir_type(t_astnode *new_node, t_token_type type)
{
    if (type == TK_LAPPEND 
        || type == TK_RAPPEND)
        new_node->data.redirection.mode = O_APPEND;
    else
        new_node->data.redirection.mode = 0;
    if (type == TK_LREDIR 
        || type == TK_LAPPEND)
        new_node->type = TK_LREDIR;
    else
        new_node->type = TK_RREDIR;
}

