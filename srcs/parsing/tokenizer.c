#include "minishell.h"
#include "parser.h"
#include <stdio.h>

// /*
//  minishell tokenizer:
//     1- split the input line into tokens
//     2- create a linked list of tokens
//     3- call the parser to parse the tokens  
// */
static t_token	*token_create(char *value, t_token_type type);
static void	token_list_append(t_token **head, t_token *to_append);
static t_token *token_list_last(t_token **token_list);
// static void determine_token_type(t_token **token);
static void tokenize(char *line, t_token **token_list);
static void escape_special_char(char *temp, unsigned int *i);
static void get_token(char *temp, unsigned int i, t_token **token_list, t_token_type type);
// static void get_quot_token(char *temp, unsigned int *i, t_token **token_list);
static int get_special_char_token(char *temp, unsigned int *i, t_token **token_list);
static int char_in_str(char c, char *str);
static int escape_quots(char *temp, unsigned int *i, t_token **token_list);
static void dollar_check(char *str, t_token **token_list, unsigned int *i);
static t_token_type get_token_type(char *str, int len);
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
    if (i != 0)
        get_token(temp, i, token_list, TK_WORD);
    if (temp[i] && !get_special_char_token(&temp[i], &i, token_list))
        return ;
    if (temp && temp[i])
        tokenize(&temp[i], token_list);
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
        dollar_check(&temp[1], token_list, i);
    else if (c == '\'')
        get_token(&temp[1], *i, token_list, TK_WORD);
    (*i)++;
    return (1);
}

void dollar_check(char *str, t_token **token_list, unsigned int *i)
{
    unsigned int j;
    unsigned int k;

    //"$'ahmed'$|'samy'"
    // $
    // 'ahmed'
    // $|'samy'
    // |'samy'
    j = 0;
    while (str[j] && str[j] != '$' && str[j] != '"')
        j++;
    if (j != 0)
        get_token(str, j, token_list, TK_WORD);
    if (str[j] == '$')
    {
        k = 1;
        escape_special_char(&str[j], &k);
        get_token(&str[j], k, token_list, TK_DOLLAR);
        j += k;
    }
    if (str[j] != '"') // need to prevent double calls
        dollar_check(&str[j], token_list, i);
}

static void get_token(char *temp, unsigned int i, t_token **token_list, t_token_type type)
{
    char *new;
    t_token *new_token;
    t_token *last_token;

    new = ft_substr(temp, 0, i);
    if (new == NULL)
        return ;
    if (token_list && (*token_list))
    {
        last_token = token_list_last(token_list);
        if ((type == TK_WORD && last_token->token_type == TK_WORD)
                || (type == TK_SPACE && last_token->token_type == TK_SPACE))
        {
            last_token->value = ft_strjoin(token_list_last(token_list)->value, new);
            if (!last_token->value)
            {
                free (new);
                // destroy_tokens; need destory
                return ;
            }
            free(new);
            return ;
        }
    }
    new_token = token_create(new, type);
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


static int get_special_char_token(char *temp, unsigned int *i, t_token **token_list)
{
    unsigned int j;

    j = 0;
    if (*temp == '"' || *temp == '\'')
    {
        if (!escape_quots(temp, &j, token_list))
            return (0);
        else
            *i += j;
    }
    else if (*temp == '$')
    {
        j = 1;
        escape_special_char(temp, &j);
        get_token(temp, j, token_list, TK_DOLLAR);
        *i += j;
    }
    else if (*(temp + 1) && *temp == *(temp + 1))
    {
        get_token(temp, 2, token_list, get_token_type(temp, 2));
        *i += 2;
    }
    else
    {
        get_token(temp, 1, token_list, get_token_type(temp, 1));
        (*i)++;
    }
    return (1);
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
static t_token	*token_create(char *value, t_token_type type)

{
    t_token	*self;

    self = malloc(sizeof(t_token));
    if (!self)
        return (NULL);
    self->prev = NULL;
    self->next = NULL;
    self->value = value;
    self->token_type = type;
    return (self);
}

// // determine the token type for each token in the list

static t_token_type get_token_type(char *str, int len)
{
    if (len == 1)
    {
        if (str[0] == '|')
            return (TK_PIPE);
        else if (str[0] == '>')
            return (TK_RREDIR);
        else if (str[0] == '<')
            return (TK_LREDIR);
        else if (str[0] == ' ')
            return (TK_SPACE);
    }
    else if (len == 2)
    {
        if (ft_strncmp(str, "||", 2) == 0)
            return (TK_OR);
        else if (ft_strncmp(str, "&&", 2) == 0)
            return (TK_AND);
        else if (ft_strncmp(str, ">>", 2) == 0)
            return (TK_RREDIR);
        else if (ft_strncmp(str, "<<", 2) == 0)
            return (TK_LREDIR);
        else if (ft_strncmp(str, "  ", 2) == 0)
            return (TK_SPACE);
    }
    return (TK_WORD);
}

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

static t_token *token_list_last(t_token **token_list)
{
    t_token *iter;

    iter = *token_list;
    while (iter && iter->next)
        iter = iter->next;
    return (iter);
}

/*
    "     w      |     w    "
    t1 -> t2 -> t3 -> t4 -> t5

*/

