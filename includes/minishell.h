/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 22:21:49 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/12 03:11:08 by ebinjama         ###   ########.fr       */
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
	EXIT_FATAL = 2,
	EXIT_NEEDED
};

/*--- LINKED - LIST ---*/

// For ease of use, always dynamically allocate for
// content.
typedef struct s_node
{
	struct s_node	*next;
	struct s_node	*prev;
	void			*content;
	bool			visible;
	bool 			is_env;
}	t_node;

// @author	Emran BinJamaan
//
// @brief	Creates a new node with the given content.
// @param	content		Content to be stored in the node.
// @return	Pointer to the newly created node. Upon failure,
//			`NULL` is returned.
// @warning	The function internally calls ft_strdup() on the content.
//			Make sure to free the content after calling this function. If
//			it were dynamically allocated.
t_node	*node_create(void *content);

// @author Emran BinJamaan
//
// @param	node	Pointer to the node to be destroyed.
// @brief	Destroys the given node, including its content.
//			If the node is part of a list, it is removed from the list, and
//			its previous and next nodes are linked together.
void	node_destroy(t_node *node);

// @author	Emran BinJamaan
//
// @param	head	 	Pointer to the head of the list.
// @param	to_append 	Pointer to the node to be appended.
// @brief	Appends the given node to the end of the list.
//			It is not required to guard against appending `NULL` nodes, because
//			the function will simply return without doing anything.
// @warning	It is up to the caller to dynamically allocate the node to append.
void	list_append(t_node **head, t_node *to_append);

// @author	Emran BinJamaan
//
// @param	head 	Pointer to the head of the list.
// @brief	Destroys the entire list, including all nodes and their content.
void	list_destroy(t_node **head);

// @author Emran BinJamaan
//
// @param	head 	Pointer to the head of the list.
// @param	strarr 		Array of strings to be duplicated to the list.
// @brief	Duplicates the given array of strings to the list.
// @return	`true` if the operation was successful, `false` otherwise.
// @warning This function assumes the list is empty, and will use list_append()
//			to add the nodes. If the list is not empty, the function will simply
//			append to it with no regard for the existing nodes. This can lead to
//			the destruction of the entire list if the operation fails. It is
//			recommended, therefore, to use this function only on an empty list;
//			and then appending that to the original one if needed.
//			Otherwise, provide a pointer to the end of the list instead
//			of the head if you don't mind potentially losing the last
//			node of the original list.
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
	TK_RAPPEND = O_APPEND,
	TK_BUILTIN,
	TK_DOLLAR,
	TK_LPAREN,
	TK_RPAREN,
	TK_AND,
	TK_OR,
	TK_DBLQT,
	TK_SGLQT,
	TK_SPACE,
	TK_DUMMY
}	t_token_type;

typedef struct s_token
{
	t_token_type			token_type;
	char					*value;
	struct s_token			*next;
	struct s_token			*prev;
}	t_token;

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
			char	*cmd;
			pid_t	pid;
			bool	isrightmost;
			t_node	*args;
			int		exit;
			bool	thereisprev;
			bool	thereispipe;
			bool	thereisout;
			bool	thereisin;
			bool	execute;
			bool	builtin;
			int		outfd;
			int		*prevfd;
			int		fd[2];
		}	command;
		struct s_redirection
		{
			char	*filename;
			int		mode;
			int		fd[2];
		}	redirection;
		struct s_pipe
		{
			bool	thereisinput;
			int		tempfd;
		} pipe;
	}	data;
}	t_astnode;

/*--- BUILTINS ---*/
t_node	*find_variable(t_node **envp, const char *variable);
int		env(t_node **envp, bool declare_flag);
int		echo(t_astnode *word, t_node *first_arg);
int		bltin_export(t_node **envp, const char *variable, const char *value);
int		unset(t_node **envp, const char *variable);
int		pwd(void);

/*--- WRAPPER FUNCTIONS ---*/
char **list_cpy_to_str_arr(t_node *lst);
int wexecve(t_astnode *word, t_node *envl, char **envp);

/*--- TEMPERORY DEBUGGING FUNCTIONS*/

void print_tokens(t_token **token);
void print_array(char **array);
void print_ast(t_astnode *ast);
void print_list(t_node **head);

/*--- DESTROY FUNCTIONS ---*/
void destroy_mini_shell(t_token **token, t_astnode **node, int exit_status);
void str_arr_destroy(char **strarr);
void destroy_str_arr(char **str_arr);
void destroy_tokens(t_token **token);
void destroy_ast(t_astnode *node);
void destroy_parser(t_token **token, t_astnode **node);

void show_error(int exit_status);
#endif // !MINISHELL_H
