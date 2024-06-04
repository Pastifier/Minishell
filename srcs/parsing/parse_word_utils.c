#include "minishell.h"
#include "parser.h"
#include <stdio.h>

int initializ_new_ast_node(t_token **token_list, t_astnode **parent)
{
    t_astnode *new_node;

    (void) token_list;
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        return (1); // need to change this to destroy the tree and exit
    new_node->type = TK_WORD;
    new_node->data.command.cmd = (*token_list)->value;
    new_node->parent = (*parent);
    new_node->left = NULL;
    new_node->right = NULL;
    *parent = new_node;
	new_node->data.command.thereisin = false;
    new_node->data.command.thereisout = false;
    new_node->data.command.thereispipe = false;
    new_node->data.command.thereisprev = false;
	new_node->data.command.isrightmost = false;
    new_node->data.command.execute = true;
    new_node->data.command.builtin = false;
    new_node->data.command.args = NULL;
    if (set_word_in_word(token_list, &new_node))
        return (1);
    return (0);
}

int set_word_in_pipe(t_token **token_list, t_astnode **node)
{
    int ret;
    t_astnode *temp;

    if ((*node) && !(*node)->right)
    {
        ret = initializ_new_ast_node(token_list, &(*node)->right);
        if (ret)
            return (1);
        (*node)->right->parent = (*node);
    }
    else if ((*node) && ((*node)->right->type == TK_RREDIR || (*node)->right->type == TK_LREDIR))
    
    {
        temp = (*node)->right;
        ret = initializ_new_ast_node(token_list, &(*node)->right);
        if (ret)
            return (1);
        (*node)->right->right = temp;
        temp->parent = (*node)->right;
    }
    else if ((*node) && (*node)->right)
        return (set_word_in_word(token_list, &(*node)->right));
    return (0);
}

int set_word_in_rredir(t_token **token_list, t_astnode **node)
{
    int ret;

    if ((*node) && !(*node)->left)
    {
        ret = initializ_new_ast_node(token_list, &(*node)->left);
        if (ret)
            return (1);
        (*node)->left->parent = (*node);
    }
    else if ((*node) && (*node)->left->type == TK_PIPE)
        return (set_word_in_pipe(token_list, &(*node)->left));
    else if ((*node) && (*node)->left)
        return (set_word_in_word(token_list, &(*node)->left));
    return (0);
}

int set_word_in_word(t_token **token_list, t_astnode **node)
{
    t_node *new_t_node;

    new_t_node = node_create((*token_list)->value);
    if (!new_t_node)
        return (1);
    if ((*token_list)->token_type == TK_DOLLAR)
        new_t_node->is_env = true;
    else
        new_t_node->is_env = false;
    list_append(&(*node)->data.command.args, new_t_node);
    return (0);
}

int set_word_in_lredir(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;
    int ret;
    
    new_node = NULL;
    ret = initializ_new_ast_node(token_list, &new_node);
    if (ret)
        return (1);
    new_node->parent = NULL;
    new_node->left = (*node);
    new_node->right = NULL;
    (*node)->parent = new_node;
    *node = new_node;
    return (0);
}
