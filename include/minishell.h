/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 22:21:49 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/02 17:49:14 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <signal.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "../libft/includes/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

/*--- ERROR - CODES ---*/
enum e_errnos
{
	EXIT_FATAL = 2
};

/*--- LINKED - LIST ---*/

// For ease of use, always dynamically allocate for
// content.
typedef struct s_node
{
	struct s_node	*next;
	struct s_node	*prev;
	void			*content;
}	t_node;

t_node	*node_create(void *content);
void	node_destroy(t_node *node);
void	list_append(t_node **head, t_node *to_append);
void	list_destroy(t_node **head);
bool	str_arr_dup_to_list(char **strarr, t_node **head);

/*--- COMMAND - STRUCT ---*/


typedef enum e_cid
{
	CD,
	ECHO,
	PWD,
	ENV,
	EXPORT,
	UNSET,
	EXIT,
}	t_cid;

typedef enum e_token_type
{
	TK_WORD,        // 0
	TK_COMMAND,     // 1
	TK_ARG,			 // 2
	TK_OPTION,		// 3	
	TK_EXEC,		 // 4
	TK_PIPE,  		// 5	
	TK_LREDIR,		 // 6
	TK_RREDIR,		// 7		
	TK_LAPPEND,		 // 8
	TK_RAPPEND,		 // 9
	TK_BUILTIN,		 // 10
	TK_D_QUOTE,		 // 11
	TK_S_QUOTE,		// 12	
	TK_ENV,			 // 13
	TK_OR,			 // 14
	TK_AND,			 // 15
}	t_token_type;

typedef struct s_astnode
{
	t_token_type		type;
	struct s_astnode	*parent;
	struct s_astnode	*left;
	struct s_astnode	*right;
	union u_data
	{
		struct s_command
		{
			char **args;
		}	command;
		struct s_redirection
		{
			char	*filename;
			int		fd;
		}	redirection;
		struct s_builtin
		{
			t_cid	id;
			char	**args;
		}	builtin;
	} data;
}	t_astnode;

typedef struct s_token
{
	t_token_type			token_type;
	char					*value;
	struct s_token			*next;
	struct s_token			*prev;
}	t_token;

// typedef struct s_cmd
// {
// 	t_cid	cmd_id;
// 	char	*line;
// 	t_split	command;
// 	t_node	**envp;
// 	t_node	**lnv;
// }	t_cmd;

/*Main usage functions*/
t_astnode	*tokenize(char *line);
void	print_ast(t_astnode *ast);

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



/*--- BUILTINS ---*/
int		env(t_node **envp);
int		export(t_node **envp, const char *variable, const char *value);
int		unset(t_node **envp, const char *variable);
int 	pwd(void);
int 	cd(char *path, t_node **envp);
int 	echo(char *str, int n_flag);


/*TEMPRORTY DEBUGGING FUNCTIONS */
void	print_tokens(t_token **token);
void	print_array(char **array);
#endif // !MINISHELL_H
