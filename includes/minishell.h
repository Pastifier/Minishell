/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 22:21:49 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/14 08:45:56 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <sys/wait.h>
# include <signal.h>

/*--- MISC. ---*/
enum e_pipedes
{
	READ_END,
	WRITE_END
};

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

typedef enum e_builtins
{
	ECHO = 0,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}	t_cid;

/*--- AST - NODE ---*/

typedef enum e_token
{
	TK_WORD,
	TK_PIPE,
	TK_LREDIR,
	TK_RREDIR,
	TK_LAPPEND,
	TK_RAPPEND,
	TK_BUILTIN,
	TK_DOLLAR,
	TK_LPAREN,
	TK_RPAREN,
	TK_AND,
	TK_OR,
	TK_DBLQT,
	TK_SGLQT
}	t_token;

typedef struct s_astnode
{
	t_token				type;
	struct s_astnode	*parent;
	struct s_astnode	*left;
	struct s_astnode	*right;
	union u_data
	{
		struct s_command
		{
			char	**args;
			int		exit;
			bool	thereispipe;
			int		fd[2];
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

/*--- BUILTINS ---*/
t_node	*find_variable(t_node **envp, const char *variable);
int		env(t_node **envp);
int		bltin_export(t_node **envp, const char *variable, const char *value);
int		unset(t_node **envp, const char *variable);

/*--- WRAPPER FUNCTIONS ---*/
char	**list_cpy_to_str_arr(t_node *lst);
void	str_arr_destroy(char **strarr);
int		wexecve(t_astnode *word, t_node *envl);

/*--- REDIRECTIONS ---*/
int	handle_lredir(const char *filename, t_astnode *node, t_node *envl);

/*--- PIPES ---*/

// @author Emran BinJamaan
// @brief creates processes for left child and right child using `fork(2)`.
// @return exit code of the right child.
// @warning this function assumes that the AST is structured perfectly, a pipe node
// requires a left and right child; it also assumes that the pipes were already
// created when `pipenode`'s children were handled. It doesn't close the
// READ_END of the pipe in `pipenode`'s right (if it exists), because that will
// be used in the pipe after it.
int	handle_pipe(t_astnode *pipenode, t_node *envl);
#endif // !MINISHELL_H
