#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/*--- MAIN - SCOPE - FUNCTIONS ---*/

int init_tokenizer(char *line, t_astnode **ast);

/*---Tokenization---*/
t_token	*token_create(char *value, t_token_type type);
void	token_list_append(t_token **head, t_token *to_append);
 t_token *token_list_last(t_token **token_list);
//  void determine_token_type(t_token **token);
int tokenize(char *line, t_token **token_list);
void escape_special_char(char *temp, unsigned int *i);

 int get_token(char *temp, unsigned int i, t_token **token_list, t_token_type type);
//  void get_quot_token(char *temp, unsigned int *i, t_token **token_list);
 int get_special_char_token(char *temp, unsigned int *i, t_token **token_list);
 int char_in_str(char c, char *str);
 int escape_quots(char *temp, unsigned int *i, t_token **token_list);
 int dollar_check(char *str, t_token **token_list, unsigned int *i);
 t_token_type get_token_type(char *str, int len);

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

/*--- PARSING - UTILS - WORD ---*/
void initializ_new_ast_node(t_token **token_list, t_astnode **parent);
void set_word_in_pipe(t_token **token_list, t_astnode **node);
void set_word_in_rredir(t_token **token_list, t_astnode **node);
void set_word_in_word(t_token **token_list, t_astnode **node);
void set_word_in_lredir(t_token **token_list, t_astnode **node);

#endif // !PARSER_H
