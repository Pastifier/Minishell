/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 22:21:49 by ebinjama          #+#    #+#             */
/*   Updated: 2024/03/21 17:36:24 by aalshafy         ###   ########.fr       */
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
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
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

typedef unsigned short	t_cid;

typedef enum e_token
{
	TK_COMMAND,
	TK_PIPE,
	TK_LREDIR,
	TK_RREDIR,
	TK_LAPPEND,
	TK_RAPPEND,
	TK_BUILTIN,
	TK_DOLLAR
}	t_token;

typedef struct s_astnode
{
	t_token	type;
	t_node	**envp;
	union u_node_data
	{
		struct s_command
		{
			char	*command;
			char	**args;
		}	command;
		struct s_pipe
		{
			struct s_astnode	*left;
			struct s_astnode	*right;
		}	pipe;
		struct s_builtin
		{
			t_cid	cmd_id;
			bool	option;
			char	*args;
		}	builtin;
	}	data;	
}	t_astnode;

typedef struct s_token
{
	char 	*token_type;
	char  	*value;
	s_token	*next;
	s_token	*prev;
}	t_token;

typedef struct s_cmd
{
	t_cid	cmd_id;
	char	*line;
	t_split	command;
	t_node	**envp;
	t_node	**lnv;
}	t_cmd;

/*--- BUILTINS ---*/
int		env(t_node **envp);
int		export(t_node **envp, const char *variable, const char *value);
int		unset(t_node **envp, const char *variable);
int 	pwd(void);
int 	cd(char *path, t_node **envp);
int 	echo(char *str, int n_flag);
#endif // !MINISHELL_H
