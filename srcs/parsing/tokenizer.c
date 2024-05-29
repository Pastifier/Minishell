#include "parser.h"
#include <stdio.h>

// /*
//  minishell tokenizer:
//     1- split the input line into tokens
//     2- create a linked list of tokens
//     3- call the parser to parse the tokens  
// */

//  void join_quot_token(char *temp, unsigned int *i, t_token **token_list);
//  t_token *token_ex(char *line);
//  void add_char_token(t_token **token_list, char *line, unsigned int *i);

int	init_tokenizer(char *line, t_astnode **ast, t_token **token_list)
{
    int ret;
    t_token     *iter;

    ret = tokenize(line, token_list);
    if (ret)
        return (ret);
    remove_spaces(token_list);
    iter = *token_list;
	ret = parse(&iter, ast);
    if (ret)
        return (ret);
    // if (ast)
    //     print_ast(*(ast));
    return (0);
}

int tokenize(char *line, t_token **token_list)
{
    char *temp;
    unsigned int i;
    int ret;

    temp = line;
    i = 0;
    if (!temp || !*temp) 
        return (0);
    escape_special_char(temp, &i);
    if (i != 0)
    {
        ret = get_token(temp, i, token_list, TK_WORD);
        if (ret)
            return (ret);
    }
    if (temp[i])
    {
        ret = get_special_char_token(&temp[i], &i, token_list);
        if (ret)
            return (ret);
    }
    if (temp && temp[i])
        return (tokenize(&temp[i], token_list));
    return (0);
}

void escape_special_char(char *temp, unsigned int *i)
{
    while (temp && temp[*i] && !char_in_str(temp[*i], " $|><&()\"'"))
        (*i)++;
}

 int escape_quots(char *temp, unsigned int *i, t_token **token_list)
{
    char c;
    int ret;

    c = temp[*i];
    (*i)++;
    while (temp[*i] && temp[*i] != c)
        (*i)++;
    if (temp[*i] == '\0')
        return (3);
    if (c == '"')
        dollar_check(&temp[1], token_list, i);
    else if (c == '\'')
    {
        ret = get_token(&temp[1], *i - 1, token_list, TK_WORD);
        if (ret)
            return (ret);
    }
    (*i)++;
    return (0);
}

int dollar_check(char *str, t_token **token_list, unsigned int *i)
{
    unsigned int j;
    unsigned int k;
    int ret;

    j = 0;
    while (str[j] && str[j] != '$' && str[j] != '"')
        j++;
    if (j != 0)
    {
        ret = get_token(str, j, token_list, TK_WORD);
        if (ret)
            return (1);
    }
    if (str[j] == '$')
    {
        k = 1;
        escape_special_char(&str[j], &k);
        ret = get_token(&str[j], k, token_list, TK_DOLLAR);
        if (ret)
            return (1);
        j += k;
    }
    if (str[j] != '"')
        return (dollar_check(&str[j], token_list, i));
    return (0);
}

//"$'ahmed'$|'samy'"
    // $
    // 'ahmed'
    // $|'samy'
    // |'samy'
