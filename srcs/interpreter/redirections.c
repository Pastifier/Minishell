#include "interpreter.h"
#include <readline/readline.h>

/*
	TODO:
		- Fix the fact that you have to not create files upon denied permission.
		- Finish the heredoc.
*/

extern int	g_signal;

int handle_lredir(t_astnode *lredir, t_shcontext *mshcontext)
{
	int			fd;
	int			pipedes[2];
	t_astnode	*closest_word;

	if (lredir->type != TK_LREDIR)
		return (EXIT_NEEDED);
	if (lredir->data.redirection.mode == O_APPEND)
		return (handle_heredoc(lredir, mshcontext));
	else if (mshcontext->permissions_clear)
	{
		closest_word = lredir->parent;
		if (closest_word)
			while (closest_word->parent && closest_word->type != TK_WORD)
				closest_word = closest_word->parent;
		if (access(lredir->data.redirection.filename, F_OK) || access(lredir->data.redirection.filename, R_OK))
		{
			if (pipe(pipedes) < 0)
				return (EXIT_FATAL);
			close(pipedes[WRITE_END]);
			dup2(pipedes[READ_END], STDIN_FILENO);
			if (closest_word)
				closest_word->data.command.execute = false;
			return (perror(lredir->data.redirection.filename),
					mshcontext->permissions_clear = false, EXIT_FAILURE);
		}
		close(STDIN_FILENO);
		fd = open(lredir->data.redirection.filename, O_RDONLY, 0755);
		if (fd < 0)
			return (EXIT_FATAL);
		if (closest_word)
			closest_word->data.command.thereisin = true;
	}
	return (EXIT_SUCCESS);
}

int handle_rredir(t_astnode *rredir, t_shcontext *mshcontext/*, int append*/)
{
	int			fd;
	int			mode;
	t_astnode	*closest_word;

	if (rredir->type != TK_RREDIR)
		return (EXIT_NEEDED);
	mode = rredir->data.redirection.mode;
	closest_word = rredir->parent;
	if (closest_word)
		while (closest_word->parent && closest_word->type != TK_WORD)
			closest_word = closest_word->parent;
	if (!access(rredir->data.redirection.filename, F_OK)
		&& access(rredir->data.redirection.filename, W_OK))
	{
		if (closest_word)
			closest_word->data.command.execute = false;
		return (perror(rredir->data.redirection.filename),
			mshcontext->permissions_clear = false, EXIT_FAILURE);
	}
	close(STDOUT_FILENO);
	mode = O_TRUNC * (mode != O_APPEND) | mode | O_WRONLY | O_CREAT;
	fd = open(rredir->data.redirection.filename, mode, 0755);
	if (fd < 0)
		return (EXIT_FATAL);
	if (closest_word)
		closest_word->data.command.thereisout = true;
	return (EXIT_SUCCESS);
}

// @brief	This function will keep reading from standard input (you'll need to re-dup stdin back, probably)
//			, and then slaps the result into a pipe. It then dups that pipe into stdin.
int handle_heredoc(t_astnode *heredoc, t_shcontext *mshcontext)
{
	t_astnode *closest_word;

	(void)mshcontext;
	closest_word = heredoc->parent;
	if (closest_word)
		while (closest_word->parent && closest_word->type != TK_WORD)
			closest_word = closest_word->parent;
	close(STDIN_FILENO);
	dup2(heredoc->data.redirection.fd[READ_END], STDIN_FILENO);
	close(heredoc->data.redirection.fd[READ_END]);
	if (closest_word)
		closest_word->data.command.thereisin = true;
	return (EXIT_SUCCESS);
}
