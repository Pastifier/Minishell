#include "minishell.h"
#include "parser.h"
#include <stdio.h>

void initializ_new_ast_node(t_token **token_list, t_astnode **parent)
{
    t_astnode *new_node;

    (void) token_list;
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        printf("malloc error\n"); // need to change this to destroy the tree and exit
    new_node->type = TK_WORD;
    new_node->data.command.cmd = (*token_list)->value;
    new_node->parent = (*parent);
    new_node->left = NULL;
    new_node->right = NULL;
    *parent = new_node;
    new_node->data.command.thereisout = false;
    new_node->data.command.thereispipe = false;
    new_node->data.command.thereisprev = false;
    new_node->data.command.args = NULL;
    set_word_in_word(token_list, &new_node);   
}

void set_word_in_pipe(t_token **token_list, t_astnode **node)
{
    if ((*node) && !(*node)->right)
    {
        initializ_new_ast_node(token_list, &(*node)->right);
        (*node)->right->parent = (*node);
    }
    else if ((*node) && (*node)->right->type == TK_PIPE)
        set_word_in_pipe(token_list, &(*node)->right);
    else if ((*node) && (*node)->right)
        set_word_in_word(token_list, &(*node)->right);
}

void set_word_in_rredir(t_token **token_list, t_astnode **node)
{
    if ((*node) && !(*node)->left)
    {
        initializ_new_ast_node(token_list, &(*node)->left);
        (*node)->left->parent = (*node);
    }
    else if ((*node) && (*node)->left->type == TK_PIPE)
        set_word_in_pipe(token_list, &(*node)->left);
    else if ((*node) && (*node)->left)
        set_word_in_word(token_list, &(*node)->left);
}

void set_word_in_word(t_token **token_list, t_astnode **node)
{
    t_node *new_t_node;

    new_t_node = node_create((*token_list)->value);
    if (!new_t_node)
        destroy_parser(token_list, node);
    if ((*token_list)->token_type == TK_DOLLAR)
        new_t_node->is_env = true;
    else
        new_t_node->is_env = false;
    list_append(&(*node)->data.command.args, new_t_node);
}

void set_word_in_lredir(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;
    
    new_node = NULL;
    initializ_new_ast_node(token_list, &new_node);
    new_node->parent = NULL;
    new_node->left = (*node);
    new_node->right = NULL;
    (*node)->parent = new_node;
    *node = new_node;
}
