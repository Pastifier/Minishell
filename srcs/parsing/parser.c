#include "minishell.h"
#include "parser.h"
#include <stdio.h>

int   parse(t_token **tokens_iter, t_astnode **node, t_node **envl)
{
    int ret;

    ret = 0;
    if ((*tokens_iter)->token_type == TK_SPACE)
        (*tokens_iter) = (*tokens_iter)->next;
    if ((*tokens_iter)->token_type == TK_WORD)
        ret = parse_word(tokens_iter, node);
    else if ((*tokens_iter)->token_type == TK_DOLLAR)
        ret = parse_env(tokens_iter, node, envl);
    else if ((*tokens_iter)->token_type == TK_PIPE)
        ret = parse_pipe(tokens_iter, node);
    else if ((*tokens_iter)->token_type == TK_LREDIR)
        ret = parse_lredir(tokens_iter, node);
    else if ((*tokens_iter)->token_type == TK_RREDIR)
        ret = parse_rredir(tokens_iter, node);
    if (ret)
        return (ret);
    if (*tokens_iter && (*tokens_iter)->next != NULL)
    {
        *tokens_iter = (*tokens_iter)->next;
        // printf("token type: %d - token value: %s\n", (*tokens_iter)->token_type, (*tokens_iter)->value);
        // printf("node type: %d\n", (*node)->type);
        return (parse(tokens_iter, node, envl));
    }
    return (0);
}

int parse_word(t_token **token_list, t_astnode **node)
{
    if (!(*node))
        return (initializ_new_ast_node(token_list, node));
    else if ((*node) && (*node)->type == TK_PIPE)
        return (set_word_in_pipe(token_list, node));
    else if ((*node) && (*node)->type == TK_RREDIR)
        return (set_word_in_rredir(token_list, node));
    else if ((*node) && (*node)->type == TK_WORD)
        return (set_word_in_word(token_list, node));
    else if ((*node) && (*node)->type == TK_LREDIR)
        return (set_word_in_lredir(token_list, node));
    return (0);
}

int parse_pipe(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;

    (void)token_list;
    if (!(*node) || ((*node)->type == TK_AND || (*node)->type == TK_OR)) 
        return (2);
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        return (1);
    new_node->type = TK_PIPE;
    (*node)->parent = new_node;
    new_node->left = *node;
    new_node->parent = NULL;
    *node = new_node;
    new_node->right = NULL;
    return (0);
}

int parse_rredir(t_token **token_list, t_astnode **node) // can be the same as parse_lredir
{
    t_astnode *new_node;
    t_astnode *iter;

    if (((*token_list)->next && (*token_list)->next->token_type != TK_WORD) || !(*token_list)->next)    
        return (2);
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        return (1);
    if ((*token_list)->token_type == TK_RAPPEND)
        new_node->data.redirection.mode = O_APPEND;
    else
        new_node->data.redirection.mode = 0;
    new_node->type = TK_RREDIR;
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->data.redirection.filename = (*token_list)->next->value;
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
    *token_list = (*token_list)->next;
    return (0);
}

int parse_lredir(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;
    t_astnode *iter;

    if (!(*token_list)->next || (*token_list)->next->token_type != TK_WORD)
        return (2);
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        return (1);
    if ((*token_list)->token_type == TK_LAPPEND)
        new_node->data.redirection.mode = O_APPEND;
    else
        new_node->data.redirection.mode = 0;
    new_node->type = TK_LREDIR;
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
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->data.redirection.filename = (*token_list)->next->value;
    *token_list = (*token_list)->next;
    return (0);
}

int parse_env(t_token **token_list, t_astnode **node, t_node **envl)
{
    t_node *iter;
    char *env_value;

    iter = *envl;
    env_value = &(*token_list)->value[1];
    while (iter && iter->next)
    {
        if (!ft_strncmp(env_value, iter->content, ft_strlen(env_value)))
        {
            free((*token_list)->value);
            (*token_list)->value = iter->content;
            return (parse_word(token_list, node));
        }
        iter = iter->next;
    }
    (*token_list)->value = "";
    return (parse_word(token_list, node));
}



