#include "minishell.h"
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

 void tokenize(char *line, t_token **token_list)
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

 void escape_special_char(char *temp, unsigned int *i)
{
    while (temp && temp[*i] && !char_in_str(temp[*i], " $|><&()\"'"))
        (*i)++;
}

 int escape_quots(char *temp, unsigned int *i, t_token **token_list)
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

