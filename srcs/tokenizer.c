# include "../include/minishell.h"

/*
 minishell tokenizer:
    1- split the input line into tokens
    2- create a linked list of tokens
    3- call the parser to parse the tokens  
*/
static t_token	*token_create(char *value);
static void	token_list_append(t_token **head, t_token *to_append);
static void determine_token_type(t_token **token);
static t_token *token_ex(char *line);
static void add_char_token(t_token **token_list, char *line, unsigned int *i);

t_astnode	*tokenize(char *line)
{
    t_token     *token_list;
    t_astnode      *ast;
    t_token     *iter;

    token_list = token_ex(line);
    if (token_list == NULL)
        return (NULL);
    determine_token_type(&token_list); // need to recheck for ||, &&, <<, >>
    print_tokens(&token_list);
    // call the parser to parse the tokens
    iter = token_list;
    ast = parse(iter);
    return (ast);
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
            iter->token_type = TK_ENV;
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
        else
            iter->token_type = TK_WORD;
        iter = iter->next;
    }
}

// split the input line into tokens by space, |, >, <, &
// determine the token type for each token
// create a linked list of tokens

static t_token *token_ex(char *line)
{
    char *temp;
    char *new;
    unsigned int i;
    t_token *token_list;
    t_token *new_token;

    temp = line;
    token_list = NULL;
    i = 0;
    while (temp[i])
    {
        if (ft_strchr(" |><&", temp[i]) || temp[i + 1] == '\0')
        {
            if (temp[i + 1] == '\0')
                i++;
            new = ft_substr(temp, 0, i);
            if (new == NULL)
                return (NULL);
            new_token = token_create(new);
            if (new_token == NULL)
            {
                destroy_tokens(&token_list);
                return (NULL);
            }
            token_list_append(&token_list, new_token);
            add_char_token(&token_list, temp, &i);
            temp += i;
            i = 0;
        }
        else
            i++;
    }
    return (token_list);
}

// add_char_token function
static void add_char_token(t_token **token_list, char *line, unsigned int *i)
{
    char *new;
    t_token *new_token;

    while (line[*i] == ' ')
        (*i)++;
    if (line[*i] && ft_strchr("|><&", line[*i]))
    {
        if (line[*i] == line[*i + 1])
        {
            new = ft_substr(line, *i, 2);
            (*i) += 2;
        }
        else
        {
            new = ft_substr(line, *i, 1);
            (*i)++;
        }
        if (new == NULL)
        {
            destroy_tokens(token_list);
            return ;
        }
        new_token = token_create(new);
        if (new_token == NULL)
            {
                destroy_tokens(token_list);
                return ;
            }
        token_list_append(token_list, new_token);
        while (line[*i] == ' ')
        (*i)++;
    }
}


