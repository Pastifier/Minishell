#include "minishell.h"
#include "parser.h"
#include <stdio.h>

// /*
//  minishell tokenizer:
//     1- split the input line into tokens
//     2- create a linked list of tokens
//     3- call the parser to parse the tokens  
// */
static t_token	*token_create(char *value);
static void	token_list_append(t_token **head, t_token *to_append);
static void determine_token_type(t_token **token);
static void tokenize(char *line, t_token **token_list);
static void escape_special_char(char *temp, unsigned int *i);
static void get_token(char *temp, unsigned int i, t_token **token_list);
// static void get_quot_token(char *temp, unsigned int *i, t_token **token_list);
static void get_special_char_token(char *temp, unsigned int *i, t_token **token_list);
static int char_in_str(char c, char *str);
static int escape_quots(char *temp, unsigned int *i);
// static void join_quot_token(char *temp, unsigned int *i, t_token **token_list);
// static t_token *token_ex(char *line);
// static void add_char_token(t_token **token_list, char *line, unsigned int *i);

t_astnode	*init_tokenizer(char *line)
{
    t_token     *token_list;
    t_astnode   *ast;
    t_token     *iter;

    token_list = NULL;
    tokenize(line, &token_list);
    determine_token_type(&token_list); // need to recheck for ||, &&, <<, >>
    // printf("tokens:\n");
    // print_tokens(&token_list);
    // call the parser to parse the tokens
    ast = NULL;
    iter = token_list;
	parse(&iter, &ast);
    return (ast);
}

static void tokenize(char *line, t_token **token_list)
{
    char *temp;
    unsigned int i;

    temp = line;
    i = 0;
    if (!temp || !*temp) 
        return ;
    escape_special_char(temp, &i);
    if (temp[i] && (temp[i] == '"' || temp[i] == '\''))
        if (!escape_quots(temp, &i))
            destroy_tokens(token_list);
    if (i != 0)
        get_token(temp, i, token_list);
    if (temp[i] != '\0' && temp[i] != ' ')
        get_special_char_token(&temp[i], &i, token_list);
    if (temp && temp[i] && temp[i + 1])
        tokenize(&temp[i + 1], token_list);
    else
        return;
}

static void escape_special_char(char *temp, unsigned int *i)
{
    while (temp && temp[*i] && char_in_str(temp[*i], " |><&()\"'$"))
        (*i)++;
}

static int escape_quots(char *temp, unsigned int *i)
{
    char c;

    c = temp[*i];
    (*i)++;
    while (temp && temp[*i] && temp[*i] != c)
        (*i)++;
    if (temp[*i] == '\0')
        return (0);
    (*i)++;
    while (temp[(*i)] && char_in_str(temp[(*i)], " \"'|><&()"))
        (*i)++;
    if (temp && temp[(*i)] && (temp[(*i)] == '"' || temp[(*i)] == '\''))
        return (escape_quots(temp, i));
    return (1);
}

static void get_token(char *temp, unsigned int i, t_token **token_list)
{
    char *new;
    t_token *new_token;

    new = ft_substr(temp, 0, i);
    if (new == NULL)
        return ;
    new_token = token_create(new);
    if (new_token == NULL)
        return ;
    token_list_append(token_list, new_token);
}

/* 
$ need to be handled
it should be a separate token
it work in double quotes
it takes the next word as an argument if it is not a special character
if it is in a double quote, and there is no argment after it, it should be considered as a word
*/


static void get_special_char_token(char *temp, unsigned int *i, t_token **token_list)
{
    if (*temp == *(temp + 1))
    {
        get_token(temp, 2, token_list);
        *i += 1;
    }
    else
        get_token(temp, 1, token_list);
}
    

// token_list append function
static void	token_list_append(t_token **head, t_token *to_append)
{
    t_token	*iter;

    if (!head || !to_append)
        return ;
    if (*head)
    {
        iter = *head;
        while (iter->next)
        {
            to_append->prev = iter;
            iter = iter->next;
        }
        iter->next = to_append;
        to_append->next = NULL;
    }
    else
        *head = to_append;
}

// token_create function
static t_token	*token_create(char *value)
{
    t_token	*self;

    self = malloc(sizeof(t_token));
    if (!self)
        return (NULL);
    self->prev = NULL;
    self->next = NULL;
    self->value = value;
    return (self);
}

// // determine the token type for each token in the list

static void determine_token_type(t_token **token)
{
    t_token *iter;

    iter = *token;
    while (iter)
    {
        if (iter->value[0] == '$')
            iter->token_type = TK_DOLLAR;
        else if (ft_strncmp(iter->value, "||", 2) == 0)
            iter->token_type = TK_OR;
        else if (ft_strncmp(iter->value, "&&", 2) == 0)
            iter->token_type = TK_AND;
        else if (ft_strncmp(iter->value, "<<", 2) == 0)
            iter->token_type = TK_LREDIR;
        else if (ft_strncmp(iter->value, ">>", 2) == 0)
            iter->token_type = TK_RREDIR;
        else if (ft_strncmp(iter->value, "|", 1) == 0)
            iter->token_type = TK_PIPE;
        else if (ft_strncmp(iter->value, ">", 1) == 0)
            iter->token_type = TK_RREDIR;
        else if (ft_strncmp(iter->value, "<", 1) == 0)
            iter->token_type = TK_LREDIR;
        else if (ft_strncmp(iter->value, "&", 1) == 0)
            iter->token_type = TK_DOLLAR;
        else if (ft_strncmp(iter->value, "(", 1) == 0)
            iter->token_type = TK_LPAREN;
        else if (ft_strncmp(iter->value, ")", 1) == 0)
            iter->token_type = TK_RPAREN;
        else if (ft_strncmp(iter->value, "\"", 1) == 0)
            iter->token_type = TK_DBLQT;
        else if (ft_strncmp(iter->value, "'", 1) == 0)
            iter->token_type = TK_SGLQT;
        else
            iter->token_type = TK_WORD;
        iter = iter->next;
    }
}

int char_in_str(char c, char *str)
{
    while (*str)
    {
        if (c == *str)
            return (0);
        str++;
    }
    return (1);
}

/*
    "     w      |     w    "
    t1 -> t2 -> t3 -> t4 -> t5

*/

