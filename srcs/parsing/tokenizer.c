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
static void get_quot_token(char *temp, unsigned int *i, t_token **token_list);
static void get_special_char_token(char *temp, unsigned int *i, t_token **token_list);
static int char_in_str(char c, char *str);
static void join_quot_token(char *temp, unsigned int *i, t_token **token_list);
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
    printf("tokens:\n");
    print_tokens(&token_list);
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
    if (i != 0)
        get_token(temp, i, token_list);
    if (temp[i] == '"' || temp[i] == '\'')
        get_quot_token(&temp[i], &i, token_list);
    else if (temp[i] != '\0' && temp[i] != ' ')
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

static void get_quot_token(char *temp, unsigned int *i, t_token **token_list)
{
    // need to handle the case where the "str1""str2" - "str"'str2' with and without $
    unsigned int j;

    j = 1;
    while (temp[j] && temp[j] != temp[0])
        j++;
    if (temp[j] == '\0')
        return ;
    get_token(temp, j, token_list);
    *i += j;
    if (temp[j + 1] && (temp[j + 1] == '"' || temp[j + 1] == '\''))
    {
        j++;
        join_quot_token(&temp[j], i, token_list);
    }
}

static void join_quot_token(char *temp, unsigned int *i, t_token **token_list)
{
    unsigned int j;
    char *new;
    char *value;
    t_token *last;

    j = 1;
    while (temp[j] && temp[j] != temp[0])
        j++;
    if (temp[j] == '\0')
        return ;
    new = ft_substr(temp, 0, j);
    if (new == NULL)
        return ; // need to destory parser
    last = (*token_list);
    while (last && last->next)
        last = last->next;
    value = last->value;
    last->value = ft_strjoin(last->value, new);
    if (last->value == NULL)
    {
        free(new);
        return ; // need to destory parser
    }
    free(new);
    free(value);
    if (temp[j + 1] && (temp[j + 1] == '"' || temp[j + 1] == '\''))
        join_quot_token(&temp[j + 1], &j, token_list);
    else
        *i += j + 1;
}

static void get_special_char_token(char *temp, unsigned int *i, t_token **token_list)
{
    printf("temp[1]: %c\n", *temp);
    printf("temp[2]: %c\n", *(temp + 1));
    if (*temp == *(temp + 1))
    {
        printf("temp2: %s\n", temp);
        get_token(temp, 2, token_list);
        *i += 2;
    }
    else
    {
        get_token(temp, 1, token_list);
        (*i)++;
    }  
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
        printf("type: %d value: %s\n", iter->token_type, iter->value);
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

// split the input line into tokens by space, |, >, <, &
// // determine the token type for each token
// // create a linked list of tokens

// static t_token *token_ex(char *line)
// {
//     char *temp;
//     char *new;
//     unsigned int i;
//     int j;
//     t_token *token_list;
//     t_token *new_token;

//     temp = line;
//     token_list = NULL;
//     i = 0;
//     while (temp[i])
//     {
//         if (ft_strchr(" |><&()\"'", temp[i]) || temp[i + 1] == '\0')
//         {
//             if (temp[i + 1] == '\0' || i == 0)
//                 i++;
//             if (temp[i] == '"' || temp[i] == '\'')
//             {
//                 j = i + 1;
//                 while (temp[j] && temp[j] != temp[i])
//                     j++;
//                 if (temp[j] == '\0')
//                     return (NULL);
//                 new = ft_substr(temp, i, j - i);
//                 if (new == NULL)
//                     return (NULL);
//                 i = j + 1;
//             }
//             else
//             {
//                 new = ft_substr(temp, 0, i);
//                 if (new == NULL)
//                     return (NULL);
//             }
//             new_token = token_create(new);
//             if (new_token == NULL)
//             {
//                 destroy_tokens(&token_list);
//                 return (NULL);
//             }
//             token_list_append(&token_list, new_token);
//             add_char_token(&token_list, temp, &i);
//             temp += i;
//             i = 0;
//         }
//         else
//             i++;
//     }
//     return (token_list);
// }

// // add_char_token function
// static void add_char_token(t_token **token_list, char *line, unsigned int *i)
// {
//     char *new;
//     t_token *new_token;

//     while (line[*i] == ' ')
//         (*i)++;
//     if (line[*i] && ft_strchr("|><&", line[*i]))
//     {
//         if (line[*i] == line[*i + 1])
//         {
//             new = ft_substr(line, *i, 2);
//             (*i) += 2;
//         }
//         else
//         {
//             new = ft_substr(line, *i, 1);
//             (*i)++;
//         }
//         if (new == NULL)
//         {
//             destroy_tokens(token_list);
//             return ;
//         }
//         new_token = token_create(new);
//         if (new_token == NULL)
//             {
//                 destroy_tokens(token_list);
//                 return ;
//             }
//         token_list_append(token_list, new_token);
//         while (line[*i] == ' ')
//         (*i)++;
//     }
// }




