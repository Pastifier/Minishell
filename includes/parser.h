/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 17:08:38 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/26 14:27:23 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/*--- MAIN - SCOPE - FUNCTIONS ---*/

int				init_tokenizer(char *line, t_astnode **ast,
					t_token **token_list, t_node **envl);

/*---Tokenization---*/
t_token			*token_create(char *value, t_token_type type);
void			token_list_append(t_token **head, t_token *to_append);
void			token_list_remove(t_token **token);
t_token			*token_list_last(t_token **token_list);
int				parse_spaces_dollars(t_token **token_list, t_node **envl);
int				tokenize(char *line, t_token **token_list);
void			escape_special_char(char *temp, unsigned int *i);
void			remove_token(t_token **token);
int				get_token(char *temp, unsigned int i, t_token **token_list,
					t_token_type type);
int				get_special_char_token(char *temp, unsigned int *i,
					t_token **token_list);
int				char_in_str(char c, char *str);
int				escape_quots(char *temp, unsigned int *i, t_token **token_list);
int				dollar_check(char *str, t_token **token_list, unsigned int *i);
t_token_type	get_token_type(char *str, int len);
int				skip_dummies(t_token **token_list);

/*--- PARSING - UTILS ---*/

char			**get_command_args(t_token **token);
t_cid			get_builtin_id(t_token **token);
int				parse(t_token **tokens_iter, t_astnode **node, t_node **envl);
int				parse_word(t_token **token_list, t_astnode **node);
int				parse_pipe(t_token **token_list, t_astnode **node);
int				parse_redir(t_token **token_list, t_astnode **node);
int				parse_lappend(t_token **token_list, t_astnode **node);
int				parse_rappend(t_token **token_list, t_astnode **node);
void			add_redir_node(t_astnode **node, t_astnode *new_node);
void			set_redir_type(t_astnode *new_node, t_token_type type);

/*--- PARSING - UTILS - WORD ---*/
int				initializ_new_ast_node(t_token **token_list,
					t_astnode **parent);
int				set_word_in_pipe(t_token **token_list, t_astnode **node);
int				set_word_in_rredir(t_token **token_list, t_astnode **node);
int				set_word_in_word(t_token **token_list, t_astnode **node);
int				set_word_in_lredir(t_token **token_list, t_astnode **node);

#endif // !PARSER_H
