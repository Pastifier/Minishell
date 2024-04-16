#ifndef INTERPRETER_H
# define INTERPRETER_H

# include "minishell.h"

// @author Emran BinJamaan
void	interpret(t_astnode *root, t_node *envl);

/*--- PREORDER FUNCTIONS ---*/

// @author Emran BinJamaan
int		prepare_pipenode(t_astnode *pipenode);

/*--- POSTORDER FUNCTIONS ---*/

// @author Emran BinJamaan
// @brief Creates processes for left child and right child using `fork(2)`,
// executes, and pipes them.
// @return Exit code of the right child. EXIT_FATAL if execution needs to stop.
// @warning this function assumes that the AST is structured perfectly, a pipe node
// requires a left and right child; it also assumes that the pipes were already
// created when `pipenode`'s children were handled. It doesn't close the
// READ_END of the pipe in `pipenode`'s right (if it exists), because that will
// be used in the pipe after it.
int		handle_pipe(t_astnode *pipenode, t_node *envl);

// @author Emran BinJamaan
// @brief fills in the struct with relevant metadata about the word (if any)
// and its subsequent execution. Executes word as command if no parent is
// present.
// @return Exit code of the word should it have executed. EXIT_FATAL if
// execution needs to stop. EXIT_SUCCESS otherwise.
int		handle_word(t_astnode *word, t_node *envl);
#endif // !INTERPRETER_H
