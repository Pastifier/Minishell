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

void   parse(t_token **tokens_iter, t_astnode **node)

{
    if ((*tokens_iter)->token_type == TK_WORD)
        parse_word(tokens_iter, node);
    else if ((*tokens_iter)->token_type == TK_PIPE)
        parse_pipe(tokens_iter, node);
    else if ((*tokens_iter)->token_type == TK_LREDIR)
        parse_lredir(tokens_iter, node);
    else if ((*tokens_iter)->token_type == TK_RREDIR)
        parse_rredir(tokens_iter, node);
    // else if (tokens_iter->token_type == TK_D_QUOTE)
    //     node = parse_dquote(&tokens_iter);
    // else if (tokens_iter->token_type == TK_S_QUOTE)
    //     node = parse_squote(&tokens_iter);
    else
        printf("syntax error\n"); // need to change this to destroy the tree and exit
    if (*tokens_iter && (*tokens_iter)->next != NULL)
    {
        *tokens_iter = (*tokens_iter)->next;
        // printf("token type: %d - token value: %s\n", (*tokens_iter)->token_type, (*tokens_iter)->value);
        // printf("node type: %d\n", (*node)->type);
        parse(tokens_iter, node);
    }
}

// parse_command will be called when the token type is command
// it will take the token as input and it should return a pointer 
// to the parse tree for the command

void parse_word(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;
    
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        printf("malloc error\n"); // need to change this to destroy the tree and exit
    // try to handle the case when the tree head is not null lrredir, rredir, pipe // but not complete
    if ((*node)->type == TK_RREDIR || (*node)->type == TK_PIPE)
    {
        new_node->parent = *node;
        new_node->right = NULL;
        new_node->left = NULL;
        if ((*node)->type == TK_RREDIR)
            (*node)->right = new_node;
        else
            (*node)->left = new_node;   
    }
    else if ((*node)->type == TK_LREDIR)
    {
        new_node->right = *node;
        new_node->parent = (*node)->parent;
        new_node->left = NULL;
        (*node)->parent = new_node;
        *node = new_node;
    }
    else if (!(*node))
    {
        new_node->parent = NULL;
        new_node->left = NULL;
        new_node->right = NULL;
        *node = new_node;
    }
    new_node->data.builtin.id = get_builtin_id(token_list);
    if (!new_node->data.builtin.id)
    {
        new_node->type = TK_WORD;
        new_node->data.command.args = get_command_args(token_list);
        if (!new_node->data.command.args)
            destroy_parser(token_list, node); // need to free the new_node
		new_node->data.command.thereispipe = false;
		new_node->data.command.thereisprev = false;
        return ;
    }
    new_node->type = TK_BUILTIN;
    new_node->data.builtin.args = get_command_args(token_list);
    if (!new_node->data.builtin.args)
        destroy_parser(token_list, node);
    if (new_node->data.builtin.id == ENV && new_node->data.builtin.args)
        destroy_parser(token_list, node);
    return;
}

// parse_pipe will be called when the token type is pipe it will take the token as input
// and check for the tree head must be a command node or a pipe node and the next node must be a command node

void parse_pipe(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;

    if ((*node)->type != TK_BUILTIN && (*node)->type != TK_PIPE 
        && (*node)->type != TK_RREDIR && (*token_list)->next->token_type != TK_WORD)
        destroy_parser(token_list, node);
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        destroy_parser(token_list, node);
    new_node->type = TK_PIPE;
    (*node)->parent = new_node;
    new_node->left = *node;
    new_node->parent = NULL;
    *node = new_node;
    *token_list = (*token_list)->next;
    parse_word(token_list, &new_node->right);
    if (new_node->right == NULL)
        destroy_parser(token_list, node);
    new_node->right->parent = new_node;
    return;
}
/*
parse_rredir will be called when the token type is rredir
the next node must be a word node
creat a new node and set the type to rredir
set the left child to the tree head
set the filename to the next node
*/
 //done but need to handle the case when the tree is empty
void parse_rredir(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;

    if ((*token_list)->next->token_type != TK_WORD)
        destroy_parser(token_list, node);
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        destroy_parser(token_list, node);
    new_node->type = TK_RREDIR;
    (*node)->parent = new_node;
    new_node->parent = NULL;
    new_node->left = *node;
    new_node->right = NULL;
    new_node->data.redirection.filename = (*token_list)->next->value;
    *node = new_node;
    *token_list = (*token_list)->next;
    return;
}

/*
parse_lredir will be called when the token type is lredir
it will take the token as input and it should return a pointer to ast
the next node must be a word node
creat a new node and set the type to rredir
reach the end of the tree and set the left child to the new node
*/
 //done but need to handle the case when the tree is empty
void parse_lredir(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;
    t_astnode *iter;

    if ((*token_list)->next->token_type != TK_WORD)
        destroy_parser(token_list, node);
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        destroy_parser(token_list, node);
    new_node->type = TK_LREDIR;
    if (!(*node))
    {
        (*node) = new_node;
        new_node->parent = NULL;
    }
    else
    {
        iter = *node;
        while (iter && iter->left)
            iter = iter->left;
        new_node->parent = iter->parent;    
        iter->left = new_node;
    }
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->data.redirection.filename = (*token_list)->next->value;
    *token_list = (*token_list)->next;
}
