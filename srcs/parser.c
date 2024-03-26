# include "../include/minishell.h"

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

t_astnode   *parse(t_token *tokens_iter)
{
    t_astnode *node;

    if (tokens_iter->token_type == TK_WORD)
        node = parse_word(&tokens_iter, &node);
    else if (tokens_iter->token_type == TK_PIPE)
        node = parse_pipe(&tokens_iter, &node);
    // else if (tokens_iter->token_type == TK_LREDIR)
    //     node = parse_lredirect(&tokens_iter);
    // else if (tokens_iter->token_type == TK_RREDIR)
    //     node = parse_rredirect(&tokens_iter);
    // else if (tokens_iter->token_type == TK_D_QUOTE)
    //     node = parse_dquote(&tokens_iter);
    // else if (tokens_iter->token_type == TK_S_QUOTE)
    //     node = parse_squote(&tokens_iter);
    else
        printf("syntax error\n"); // need to change this to destroy the tree and exit
    if (tokens_iter->next != NULL)
    {
        printf("next token type: %d\n", tokens_iter->next->token_type);
        node = parse(tokens_iter->next);
    }
    return (node);
}
    
    // printf("parse\n");
    // printf("token type: %d\n", (*token_list)->token_type);
    // if ((*token_list)->token_type == TK_WORD)
    //     node = parse_word(&token_list, &node);
    // else if ((*token_list)->token_type == TK_PIPE)
    //     node = parse_pipe(&token_list, &node);
    // else if ((*token_list)->token_type == TK_LREDIR)
    //     node = parse_lredirect(token_list);
    // else if ((*token_list)->token_type == TK_RREDIR)
    //     node = parse_rredirect(token_list);
    // else if ((*token_list)->token_type == TK_D_QUOTE)
    //     node = parse_dquote(token_list);
    // else if ((*token_list)->token_type == TK_S_QUOTE)
    //     node = parse_squote(token_list);
    // else
    //     printf("syntax error\n"); // need to change this to destroy the tree and exit
    // if ((*token_list)->next != NULL)
    //     node = parse(token_list->next);
    // return (node);


// parse_command will be called when the token type is command
// it will take the token as input and it should return a pointer 
// to the parse tree for the command

t_astnode *parse_word(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;
    
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        printf("malloc error\n"); // need to change this to destroy the tree and exit
    new_node->data.builtin.id = get_builtin_id(token_list);
    if (!new_node->data.builtin.id)
    {
        new_node->type = TK_EXEC;
        new_node->data.command.args = get_command_args(token_list);
        if (!new_node->data.command.args)
            destroy_parser(token_list, node); // need to free the new_node
        return (new_node);
    }
    new_node->type = TK_BUILTIN;
    new_node->data.builtin.args = get_command_args(token_list);
    if (!new_node->data.builtin.args)
        destroy_parser(token_list, node);
    if (new_node->data.builtin.id == ENV && new_node->data.builtin.args)
        destroy_parser(token_list, node);
    // need to make the pointer to the next token and to the partent node
    return (new_node);
}

// fill command node will be called in parse_command it will take the node and token as input
// it will check the command type and parse the command accordingly to its grammer rules
// and fill the node with the parsed data or destroy the tree and exit if there is a syntax error

// 23-3 added parse builtins function to parse the builtins
// need to check if other command can added to the parse builtins function
// or if we need to create a new function for each command
// also need to check for exit errors

// parse_pipe will be called when the token type is pipe it will take the token as input
// and check for the tree head must be a command node or a pipe node and the next node must be a command node

t_astnode *parse_pipe(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;

    if ((*node)->type != TK_COMMAND && (*node)->type != TK_PIPE 
        && (*node)->type != TK_RREDIR && (*token_list)->next->token_type != TK_WORD)
        destroy_parser(token_list, node);
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        destroy_parser(token_list, node);
    new_node->type = TK_PIPE;
    new_node->left = *node;
    new_node->right = parse((*token_list)->next);
    if (new_node->right == NULL)
        destroy_parser(token_list, node);
    return (new_node);
}
