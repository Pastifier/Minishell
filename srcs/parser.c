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

t_astnode   *parse(t_token **tokens_iter, t_astnode **node)

{
    if ((*tokens_iter)->token_type == TK_WORD)
        parse_word(tokens_iter, node);
    else if ((*tokens_iter)->token_type == TK_PIPE)
        parse_pipe(tokens_iter, node);
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
    if ((*tokens_iter)->next != NULL)
    {
        *tokens_iter = (*tokens_iter)->next;
        // printf("token type: %d - token value: %s\n", (*tokens_iter)->token_type, (*tokens_iter)->value);
        parse(tokens_iter, node);
    }
    return (*node);
}

// parse_command will be called when the token type is command
// it will take the token as input and it should return a pointer 
// to the parse tree for the command

void parse_word(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;
    
    printf("parse_word: %s\n", (*token_list)->value);
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        printf("malloc error\n"); // need to change this to destroy the tree and exit
    *node = new_node;
    new_node->data.builtin.id = get_builtin_id(token_list);
    if (!new_node->data.builtin.id)
    {
        new_node->type = TK_EXEC;
        new_node->data.command.args = get_command_args(token_list);
        printf("command args DONE: %s - type: %d\n", new_node->data.command.args[0], new_node->type);
        printf("????????" );
        if (!new_node->data.command.args)
            destroy_parser(token_list, node); // need to free the new_node
        return ;
    }
    new_node->type = TK_BUILTIN;
    new_node->data.builtin.args = get_command_args(token_list);
    printf("command args: %s\n", new_node->data.builtin.args[0]);
    printf("new_token: %s\n", (*token_list)->value);
    if (!new_node->data.builtin.args)
        destroy_parser(token_list, node);
    if (new_node->data.builtin.id == ENV && new_node->data.builtin.args)
        destroy_parser(token_list, node);
    return;
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

void parse_pipe(t_token **token_list, t_astnode **node)
{
    t_astnode *new_node;

    printf("parse_pipe %s\n", (*token_list)->value);
    if ((*node)->type != TK_COMMAND && (*node)->type != TK_PIPE 
        && (*node)->type != TK_RREDIR && (*token_list)->next->token_type != TK_WORD)
        destroy_parser(token_list, node);
    new_node = (t_astnode *)malloc(sizeof(t_astnode));
    if (new_node == NULL)
        destroy_parser(token_list, node);
    new_node->type = TK_PIPE;
    new_node->left = *node;

    node = &new_node;
    // print_ast(new_node->left);
    *token_list = (*token_list)->next;
    parse_word(token_list, &new_node->right);
    if (new_node->right == NULL)
        destroy_parser(token_list, node);
    printf("PIPE DONE\n");
    return;
}
