# include "parser.h"
# include <stdio.h>

int get_token(char *temp, unsigned int i, t_token **token_list, t_token_type type)
{
    char *new;
    char *last_value;
    t_token *new_token;
    t_token *last_token;

    new = ft_substr(temp, 0, i);
    if (new == NULL)
        return (1);
    if (ft_strncmp(new, "\"\"", 2) == 0 || ft_strncmp(new, "''", 2) == 0)
    {
        free(new);
        new = ft_strdup("");
        if (new == NULL)
            return (1);
    }
    if (token_list && (*token_list))
    {
        last_token = token_list_last(token_list);
        if ((type == TK_WORD && last_token->token_type == TK_WORD)
                || (type == TK_SPACE && last_token->token_type == TK_SPACE))
        {
            last_value = last_token->value;
            last_token->value = ft_strjoin(last_value, new);
            if (!last_token->value)
                return (free (new), free(last_value), 1);
            return (free(new), free(last_value), 0);
        }
    }
    new_token = token_create(new, type);
    if (new_token == NULL)
        return (free(new), 1);
    token_list_append(token_list, new_token);
    return (0);
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
    int          ret;

    j = 0;
    if (*temp == '"' || *temp == '\'')
    {
        if (escape_quots(temp, &j, token_list))
            return (3);
        else
            *i += j;
    }
    else if (*temp == '$')
    {
        j = 1;
        escape_special_char(temp, &j);
        ret = get_token(temp, j, token_list, TK_DOLLAR);
        if (ret)
            return (ret);
        *i += j;
    }
    else if (*(temp + 1) && *temp == *(temp + 1))
    {
        ret = get_token(temp, 2, token_list, get_token_type(temp, 2));
        if (ret)
            return (ret);
        *i += 2;
    }
    else
    {
        ret = get_token(temp, 1, token_list, get_token_type(temp, 1));
        if (ret)
            return (ret);
        (*i)++;
    }
    return (0);
}
int     parse_spaces_dollars(t_token **token_list, t_node **envl)
{
    t_token *iter;
    int     ret;

    iter = *token_list;
    while (iter)
    {
        if (iter->token_type == TK_DOLLAR)
        {
            ret = parse_env(&iter, envl); // I think this is the probelm related the 
            // to remove_token function as i passing the address of iter which is locally
            // declared in this function
            if (ret)
                return (ret);
        }
        else
            iter = iter->next;
    }
    if (*token_list == NULL)
        return (0);
    iter = *token_list;
    while (iter)
    {
        if (iter->token_type == TK_SPACE)
            remove_token(&iter);
        else
            iter = iter->next;
    }   
    return (0);
}

int parse_env(t_token **token_list, t_node **envl)
{
    t_node *iter;
	char *eql_addr;
    char *env_value;

    iter = *envl;
    (*token_list)->token_type = TK_WORD;
    if (!(*token_list)->value[1])
        return (0);
    env_value = &(*token_list)->value[1];
	if ((*token_list)->value[1] == '?') // Emran
	{
		env_value = (*token_list)->value;
        (*token_list)->value = ft_itoa(*(int *)iter->content);
        if (!(*token_list)->value)
            return (free(env_value), 1);
        if (env_value[2])
        {
            (*token_list)->value = ft_strjoin((*token_list)->value, env_value + 2);
            if (!(*token_list)->value)
                return (free(env_value), 1);
        }
        return (free(env_value), join_env(token_list));
	}
	while (iter)
    {
        if (!ft_strncmp(env_value, iter->content, ft_strlen(env_value)))
        {
            free((*token_list)->value);
            eql_addr = ft_strchr(iter->content, '=');
            (*token_list)->value = ft_strdup(eql_addr + 1);
            if (!(*token_list)->value)
                return (1);
            return (join_env(token_list));
        }
        iter = iter->next;
    }
    remove_token(token_list);
    // free((*token_list)->value);
    // (*token_list)->value = ft_strdup(""); // Emran
    // if (!(*token_list)->value)
    //     return (1);
    
    return (0);
}

int join_env(t_token **token_list)
{
    char *content;
    t_token *iter;

    iter = *token_list;
    if ((*token_list)->prev && (*token_list)->prev->token_type == TK_WORD)
    {
        content = iter->prev->value;
        iter->prev->value = ft_strjoin(content, iter->value);
        if (!iter->prev->value)
            return (1);
        remove_token(&iter);
        (*token_list) = iter;
        free(content);
    }
    else
        (*token_list) = iter->next;
    return (0);
}
