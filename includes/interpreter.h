#ifndef INTERPRETER_H
# define INTERPRETER_H

# include "minishell.h"
# include <signal.h>

#ifdef __linux__
# define EXIT_INVAL_ARG		2
#endif // !__LINUX__

#ifdef __MACH__
# define EXIT_INVAL_ARG		255
#endif // !__MACH__

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
// @brief	Resets the standard file-descriptors in order to emulate bash behaviour
//			more accurately.
int		handle_pipe(t_astnode *pipenode, t_shcontext *mshcontext);

// @author Emran BinJamaan
// @brief Executes word depending on its metadata provided by the `prepare_*()` functions.
// @return Exit code of the word should it have executed. EXIT_FATAL if
// execution needs to stop. EXIT_SUCCESS otherwise.
int		handle_word(t_astnode *word, t_node *envl, t_shcontext *mshcontext);

// @author Emran BinJamaan
// @brief	Handles left redirections.
// @warning	This function assumes that the AST is structured perfectly,
//			i.e, input redirections are children to words.
int 	handle_lredir(t_astnode *lredir, t_shcontext *mshcontext);

// @author Emran BinJamaan
int 	handle_rredir(t_astnode *rredir, t_shcontext *mshcontext);

// @author Emran BinJamaan
int		handle_heredoc(t_astnode *heredoc, t_shcontext *mshcontext);

// @author Emran BinJamaan
// @brief  wrapper for the `cd` built-in.
int		wcd(t_astnode *cdnode, t_shcontext *mshcontext);

// @author Ahmed Alshafy
// @brief  wrapper for the `export` built-in.
int		parse_export(const char *var_name); // I told you to make it const, bro :(

// @author Emran BinJamaan
int		wexport(t_astnode *word, t_node **envp, const char *variable,
	const char *value);

// @author Emran BinJamaan
int		wunset(t_astnode *word, t_node **envp);

#endif // !INTERPRETER_H
