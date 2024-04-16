#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/*--- MAIN - SCOPE - FUNCTIONS ---*/

t_astnode	*tokenize(char *line);

/*--- PARSING - UTILS ---*/

char **get_command_args(t_token **token);
char *ft_str_toupper(char *str);
t_cid get_builtin_id(t_token **token);
void destroy_str_arr(char **str_arr);
void destroy_tokens(t_token **token);
void destroy_ast(t_astnode *node);
void destroy_parser(t_token **token, t_astnode **node);
void   parse(t_token **tokens_iter, t_astnode **node);
void parse_word(t_token **token_list, t_astnode **node);
void parse_pipe(t_token **token_list, t_astnode **node);
void parse_rredir(t_token **token_list, t_astnode **node);
void parse_lredir(t_token **token_list, t_astnode **node);
void parse_lappend(t_token **token_list, t_astnode **node);
void parse_rappend(t_token **token_list, t_astnode **node);

#endif // !PARSER_H
