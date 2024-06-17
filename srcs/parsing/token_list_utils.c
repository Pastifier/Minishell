#include "parser.h"
#include <stdio.h>


// token_list append function
void	token_list_append(t_token **head, t_token *to_append)
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
        to_append->prev = iter;
        to_append->next = NULL;
    }
    else
        *head = to_append;
}

// token_create function
t_token	*token_create(char *value, t_token_type type)

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

t_token_type get_token_type(char *str, int len)
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
            return (TK_RAPPEND);
        else if (ft_strncmp(str, "<<", 2) == 0)
            return (TK_LAPPEND);
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

t_token *token_list_last(t_token **token_list)
{
    t_token *iter;

    iter = *token_list;
    while (iter && iter->next)
        iter = iter->next;
    return (iter);
}

void remove_token(t_token **token_list)
{
    t_token *iter;
    t_token *temp;

    iter = *token_list;
    temp = iter;
    if (!iter->next && !iter->prev)
        *token_list = NULL;
    else if (!iter->prev)
    {
        *token_list = iter->next;
        (*token_list)->prev = iter->prev;
    }
    else if (!iter->next)
    {
        iter->prev->next = NULL;
        (*token_list) = NULL;
    }
    else
    {
        iter->prev->next = iter->next;
        iter->next->prev = iter->prev;
        (*token_list) = iter->next;
    }
    free(temp->value);
    free(temp);
    temp = NULL;
}
