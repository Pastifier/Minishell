# include "../include/minishell.h"

/*
 minishell tokenizer:
    1- split the input line into tokens
    2- create a linked list of tokens
    3- call the parser to parse the tokens  
*/

t_astnode	*tokenize(char *line)
{
    t_token     *token_list;
    t_token     *new_token;
    t_split	        split;
    char        **tokens;
    t_astnode      *ast;
    int		i;

    token_list = NULL;
    split = ft_split(line, " |><&");
    tokens = split.array;
    if (tokens == NULL)
        return ;
    i = 0;
    while (tokens[i] != NULL)
    {
        new_token = token_create(tokens[i]);
        if (new_token == NULL)
        {
            destory_str_arr(tokens);
            destroy_tokens(&token_list); // need to implement this function
            return ;
        }
        token_list_append(&token_list, token_create(tokens[i]));
        i++;
    }
    destory_str_arr(tokens); 
    token_type(&token_list); // need to recheck for ||, &&, <<, >>
    // call the parser to parse the tokens
    ast = parse(&token_list);
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

// determine the token type for each token in the list

static void determine_token_type(t_token **token)
{
    t_token *iter;

    iter = *token;
    while (iter)
    {
        if (iter->value[0] == '$')
            iter->token_type = "ENV";
        else if (iter->value[0] == '|')
            iter->token_type = "PIPE";
        else if (iter->value[0] == '>')
            iter->token_type = "REDIR";
        else if (iter->value[0] == '<')
            iter->token_type = "REDIR";
        else
            iter->token_type = "CMD";
        iter = iter->next;
    }
}

// destroy the token list
// static void	ft_destroy_token_list(t_token **head)
// {
//     t_token	*temp;
//     t_token	*iter;

//     if (!head)
//         return ;
//     iter = *head;
//     while (iter)
//     {
//         temp = iter;
//         iter = iter->next;
//         free(temp);
//     }
//     *head = NULL;
// }