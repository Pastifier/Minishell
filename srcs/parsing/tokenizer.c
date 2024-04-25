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
// static void determine_token_type(t_token **token);
static void tokenize(char *line, t_token **token_list);
static void escape_special_char(char *temp, unsigned int *i);
static void get_token(char *temp, unsigned int i, t_token **token_list);
// static void get_quot_token(char *temp, unsigned int *i, t_token **token_list);
static void get_special_char_token(char *temp, unsigned int *i, t_token **token_list);
static int char_in_str(char c, char *str);
static int escape_quots(char *temp, unsigned int *i, t_token **token_list);
static void dollar_check(char *str, t_token **token_list, unsigned int *i);
// static void join_quot_token(char *temp, unsigned int *i, t_token **token_list);
// static t_token *token_ex(char *line);
// static void add_char_token(t_token **token_list, char *line, unsigned int *i);

t_astnode	*init_tokenizer(char *line)
{
    t_token     *token_list;
    t_astnode   *ast;
    // t_token     *iter;

    token_list = NULL;
    tokenize(line, &token_list);
    print_tokens(&token_list);
    // determine_token_type(&token_list); // need to recheck for ||, &&, <<, >>
    // // printf("tokens:\n");
    // // print_tokens(&token_list);
    // // call the parser to parse the tokens
    ast = NULL;
    // iter = token_list;
	// parse(&iter, &ast);
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
    if (i == 0 && (temp[i] == '"' || temp[i] == '\'') && !escape_quots(temp, &i, token_list))
        destroy_tokens(token_list);
    else if (i != 0)
        get_token(temp, i, token_list);
    if (temp[i] && temp[i] != '$' && temp[i] != '"' && temp[i] != '\'') // need to check for special characters
        get_special_char_token(&temp[i], &i, token_list);
    if (temp && temp[i] && temp[i + 1])
        tokenize(&temp[i + 1], token_list);
    else
        return;
}

static void escape_special_char(char *temp, unsigned int *i)
{
    while (temp && temp[*i] && !char_in_str(temp[*i], " $|><&()\"'"))
        (*i)++;
}

static int escape_quots(char *temp, unsigned int *i, t_token **token_list)
{
    char c;

    c = temp[*i];
    (*i)++;
    while (temp[*i] && temp[*i] != c)
        (*i)++;
    if (temp[*i] == '\0')
        return (0);
    if (c == '"')
        dollar_check(temp, token_list, i);
    else if (c == '\'')
        get_token(temp, *i, token_list);
    (*i)++;
    return (1);
}

void dollar_check(char *str, t_token **token_list, unsigned int *i)
{
    unsigned int j;
    unsigned int k;

    j = 0;
    while (str[j] && str[j] != '$' && j != *i)
        j++;
    get_token(str, j + 1, token_list);
    if (str[j] == '$')
    {
        k = j;
        escape_special_char(str, &j);
        get_token(&str[k], j, token_list);
    }
    if (j != *i)
        dollar_check(&str[j], token_list, i);
}

static void get_token(char *temp, unsigned int i, t_token **token_list)
{
    char *new;
    t_token *new_token;

    printf("temp: i = %d\n", i);
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
    printf("temp: %s\n", temp);
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

// static void determine_token_type(t_token **token)
// {
//     t_token *iter;

//     iter = *token;
//     while (iter)
//     {
//         if (iter->value[0] == '$')
//             iter->token_type = TK_DOLLAR;
//         else if (ft_strncmp(iter->value, "||", 2) == 0)
//             iter->token_type = TK_OR;
//         else if (ft_strncmp(iter->value, "&&", 2) == 0)
//             iter->token_type = TK_AND;
//         else if (ft_strncmp(iter->value, "<<", 2) == 0)
//             iter->token_type = TK_LREDIR;
//         else if (ft_strncmp(iter->value, ">>", 2) == 0)
//             iter->token_type = TK_RREDIR;
//         else if (ft_strncmp(iter->value, "|", 1) == 0)
//             iter->token_type = TK_PIPE;
//         else if (ft_strncmp(iter->value, ">", 1) == 0)
//             iter->token_type = TK_RREDIR;
//         else if (ft_strncmp(iter->value, "<", 1) == 0)
//             iter->token_type = TK_LREDIR;
//         else if (ft_strncmp(iter->value, "&", 1) == 0)
//             iter->token_type = TK_DOLLAR;
//         else if (ft_strncmp(iter->value, "(", 1) == 0)
//             iter->token_type = TK_LPAREN;
//         else if (ft_strncmp(iter->value, ")", 1) == 0)
//             iter->token_type = TK_RPAREN;
//         else if (ft_strncmp(iter->value, "\"", 1) == 0)
//             iter->token_type = TK_DBLQT;
//         else if (ft_strncmp(iter->value, "'", 1) == 0)
//             iter->token_type = TK_SGLQT;
//         else if (ft_strncmp(iter->value, "$", 1) == 0)
//             iter->token_type = TK_ENV;
//         else
//             iter->token_type = TK_WORD;
//         iter = iter->next;
//     }
// }

int char_in_str(char c, char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return (1);
        i++;
    }
    return (0);
}

/*
    "     w      |     w    "
    t1 -> t2 -> t3 -> t4 -> t5

*/

