# include "minishell.h"
# include "parser.h"

void get_token(char *temp, unsigned int i, t_token **token_list, t_token_type type)
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


 int get_special_char_token(char *temp, unsigned int *i, t_token **token_list)
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
    


/*
    "     w      |     w    "
    t1 -> t2 -> t3 -> t4 -> t5

*/

