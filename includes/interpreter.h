/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 09:53:03 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/21 09:11:36 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERPRETER_H
# define INTERPRETER_H

# include "minishell.h"
# include <signal.h>

# define OS_IS_MAC false

# ifdef __linux__
#  define EXIT_INVAL_ARG		2
# endif // !__LINUX__

# ifdef __MACH__
#  undef OS_IS_MAC
#  define OS_IS_MAC true
#  define EXIT_INVAL_ARG		255
# endif // !__MACH__

/* --- LOG - MESSAGES ---*/

# define CD_INVAL_USE "msh: invalid use of `cd`. Read the subject, habibi."
// HD stands for "Here Document"
# define HD "%s: %s: %s delimited by end-of-file (wanted `%s')\n"
# define NO_HOME "msh: cd: HOME not set"
# define NO_OLDPWD "msh: cd: OLDPWD not set"
# define SV "STOP! You have violated the segments. "
# define SV2 "Your stolen bytes are now forfeit!"

typedef struct sigaction	t_sigaction;
typedef struct s_tree
{
	t_node				*envl;
	bool				terminate;
	bool				permissions_clear;
	t_astnode			*root;
	t_astnode			*last_command;
	t_astnode			*rightmost_word;
	int					wstatus;
	int					exit_status;
	t_sigaction			*sa;
	t_sigaction			*oldact;
	int					stds[3];
	char				**allocated_envp;
}	t_shcontext;

// @author Emran BinJamaan
int		interpret(t_astnode *root, t_node *envl);

void	restore_iodes(t_shcontext *mshcontext, bool clear);

// @author Emran BinJamaan
void	visit_prematurely(t_astnode *node, t_shcontext *mshcontext);

/*--- PREORDER FUNCTIONS ---*/

// @author Emran BinJamaan
int		prepare_pipenode(t_astnode *pipenode, t_shcontext *mshcontext);

// @author Emran BinJamaan
int		prepare_heredoc(t_astnode *lredir, t_shcontext *mshcontext);

/*--- POSTORDER FUNCTIONS ---*/

// @author Emran BinJamaan
// @brief	Resets the standard file-descriptors in order to
//			emulate bash behaviour more accurately.
int		handle_pipe(t_astnode *pipenode, t_shcontext *mshcontext);

// @author Emran BinJamaan
// @brief Executes word depending on its metadata provided by the
//		 `prepare_*()` functions.
// @return Exit code of the word should it have executed. EXIT_FATAL if
// execution needs to stop. EXIT_SUCCESS otherwise.
int		handle_word(t_astnode *word, t_node *envl, t_shcontext *mshcontext);

// @author Emran BinJamaan
// @brief	Handles left redirections.
// @warning	This function assumes that the AST is structured perfectly,
//			i.e, input redirections are children to words.
int		handle_lredir(t_astnode *lredir, t_shcontext *mshcontext);

// @author Emran BinJamaan
int		handle_rredir(t_astnode *rredir, t_shcontext *mshcontext);

// @author Emran BinJamaan
int		handle_heredoc(t_astnode *heredoc, t_shcontext *mshcontext);

// @author Emran BinJamaan
int		execute_builtin(t_astnode *word, t_shcontext *mshcontext);

// @author Emran BinJamaan
// @brief  wrapper for the `cd` built-in.
int		wcd(t_astnode *cdnode, t_shcontext *mshcontext);
int		cd(char *path, t_node **envp);
int		check_home_and_cd_if_it_exists(t_node **envp);
int		check_oldpwd_and_cd_if_it_exists(t_node **envp);

// @author Ahmed Alshafy
// @brief  wrapper for the `export` built-in.
int		parse_export(const char *var_name);

// @author Emran BinJamaan
int		wunset(t_astnode *word, t_node **envp);

/*--- NORME - DUMP ---*/

// @author Emran BinJamaan
void	perform_wait_and_fetch_wstatus(t_shcontext *mshcontext);
int		determine_exit_code(t_shcontext *mshcontext);
int		store_heredoc_input(t_astnode *lredir, int *pipedes,
			t_shcontext *mshcontext);
void	perform_word_checks_and_close_pipes_if_needed(t_astnode *word,
			t_shcontext *mshcontext, char **envp, t_node *envl);
void	ignore_signals_and_close_pipes_if_needed_then_set_pid(t_astnode *word,
			pid_t pid, char **envp);
int		wexit(t_astnode *word, t_shcontext *mshcontext, char *first_arg);
void	init_builtin_necessities(t_astnode *word, char **variable, char **temp,
			char **first_arg);
int		w_wexport(t_astnode *word, t_node **envp);
char	*perform_joining_of_var_and_val(const char *variable, const char *value);
int		confirm_var_visibility(size_t val_length, t_node *varp, void *content);
#endif // !INTERPRETER_H
