#include "minishell.h"
#include "parser.h"
#include <stdio.h>

/* 
recursive decent parser:
    - parse the input string
    - check for syntax errors
    - build a parse tree
    - generate code
    - execute the code

the parser function will be called after the tokenization
is done, 
it will take the tokenized string as input
and it should return a pointer to the parse tree
which can then be used by other functions in this
program to generate code and execute it.
*/

int   parse(t_token **tokens_iter, t_astnode **node)

{
    int ret;

    ret = 0;
    if ((*tokens_iter)->token_type == TK_SPACE)
        (*tokens_iter) = (*tokens_iter)->next;
    if ((*tokens_iter)->token_type == TK_WORD)
        ret = parse_word(tokens_iter, node);
    else if ((*tokens_iter)->token_type == TK_DOLLAR)
        ret = parse_word(tokens_iter, node);
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
        return (parse(tokens_iter, node));
    }
    return (0);
}

// parse_command will be called when the token type is command
// it will take the token as input and it should return a pointer 
// to the parse tree for the command

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

// parse_pipe will be called when the token type is pipe it will take the token as input
// and check for the tree head must be a command node or a pipe node and the next node must be a command node

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
/*
parse_rredir will be called when the token type is rredir
the next node must be a word node
creat a new node and set the type to rredir
set the left child to the tree head  
set the filename to the next node
*/
 //done but need to handle the case when the tree is empty
int parse_rredir(t_token **token_list, t_astnode **node) // can be the same as parse_lredir
{
    t_astnode *new_node;
    t_astnode *iter;

    if (((*token_list)->next && (*token_list)->next->token_type != TK_WORD) || !(*token_list)->next)    
        return (2); // destory need fixes & gards
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        return (1);
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

/*
parse_lredir will be called when the token type is lredir
it will take the token as input and it should return a pointer to ast
the next node must be a word node
creat a new node and set the type to rredir
reach the end of the tree and set the left child to the new node
*/
 //done but need to handle the case when the tree is empty
int parse_lredir(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;
    t_astnode *iter;

    if (!(*token_list)->next || (*token_list)->next->token_type != TK_WORD)
        return (2);
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        return (1);
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



