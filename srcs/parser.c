# include "minishell.h"

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

void   parse(t_token *tokens)
{
    t_token *token;
    
    token = tokens;
    if (token->token_type == str)
        parse_command(token);
    else if (token->token_type == pipe)
        parse_pipe(token);
    else if (token->token_type == redirect)
        parse_redirect(token);    
    else
        error("syntax error");
    if (token->next != NULL)
        parse(token->next);
}

// parse_command will be called when the token type is command
// it will take the token as input and it should return a pointer 
// to the parse tree for the command

t_astnode *parse_command(t_token *token)
{
    t_astnode *node;
    t_astnode *args;
    t_astnode *command;
    
    node = (t_astnode *)malloc(sizeof(t_astnode));
    if (node == NULL)
        error("malloc failed");
    node->type = command;
    command = (t_astnode *)malloc(sizeof(t_astnode));
    if (command == NULL)
        error("malloc failed");
    fill_command_node(node, token);
    // command->type = put_command_type(token->value);
    // command->command = token->value;
    // args = parse_args(token->next);
    // node->command = command;
    // node->args = args;
    return (node);
}

// fill command node will be called in parse_command it will take the node and token as input
// it will check the command type and parse the command accordingly to its grammer rules
// and fill the node with the parsed data or destory the tree and exit if there is a syntax error

void fill_command_node(t_astnode *node, t_token *token)
{
    if (strcmp(command, "echo") == 0) // need to edit the case
        parse_echo(node, token); // check for echo args
    else if (strcmp(command, "cd") == 0) // work in small only
        parse_cd(node, token); // check for cd args and grammer rules "cd untitled\ folder/ echo ahmed && echo hello"
    else if (strcmp(command, "pwd") == 0) // need to edit the case
        parse_pwd(node, token); // check for pwd args and grammer rules
    else if (strcmp(command, "export") == 0)
        parse_export(node, token);
    else if (strcmp(command, "unset") == 0)
        parse_unset(node, token);
    else if (strcmp(command, "env") == 0)
        parse_env(node, token);
    else if (strcmp(command, "exit") == 0)
        parse_exit(node, token);
    else
        parse_exc(node, token);
}