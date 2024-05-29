#ifndef INTERPRETER_H
# define INTERPRETER_H

# include "minishell.h"

typedef struct s_tree
{
	t_node		*envl;
	bool		terminate;
	bool		permissions_clear;
	t_astnode	*root;
	t_astnode	*last_command;
	t_astnode	*rightmost_word;
	int			wstatus;
	int			exit_status;
	int			stds[2];
}	t_shcontext;

// @author Emran BinJamaan
int	interpret(t_astnode *root, t_node *envl);

/*--- PREORDER FUNCTIONS ---*/

// @author Emran BinJamaan
// @brief Opens `filename` and stores its fd in the concerned word, so that
// the word can redirect its output to that fd.
int	prepare_rredir(t_astnode *rredir);

// @author Emran BinJamaan
int	prepare_pipenode(t_astnode *pipenode, t_shcontext *mshcontext);

/*--- POSTORDER FUNCTIONS ---*/

// [ DEPRECATED ]
// @author Emran BinJamaan
// @brief Creates processes for left child and right child using `fork(2)`,
// executes, and pipes them.
// @return Exit code of the right child. EXIT_FATAL if execution needs to stop.
// @warning this function assumes that the AST is structured perfectly, a pipe node
// requires a left and right child; it also assumes that the pipes were already
// created when `pipenode`'s children were handled. It doesn't close the
// READ_END of the pipe in `pipenode`'s right (if it exists), because that will
// be used in the pipe after it.
// [ DEPRECATED ]

// @author Emran BinJamaan
// @brief	Resets the standard file-descriptors in order to emulate bash behaviour
//			more accurately.
int	handle_pipe(t_astnode *pipenode, t_shcontext *mshcontext);

// @author Emran BinJamaan
// @brief Executes word depending on its metadata provided by the `prepare_*()` functions.
// @return Exit code of the word should it have executed. EXIT_FATAL if
// execution needs to stop. EXIT_SUCCESS otherwise.
int	handle_word(t_astnode *word, t_node *envl);

// @author Emran BinJamaan
// @brief	...
// @warning	This function assumes that the AST is structured perfectly,
//			i.e, input redirections are to the left of words.
int handle_lredir(t_astnode *lredir, t_shcontext *mshcontext);

// @author Emran BinJamaan
int handle_rredir(t_astnode *rredir, t_shcontext *mshcontext);

#endif // !INTERPRETER_H
