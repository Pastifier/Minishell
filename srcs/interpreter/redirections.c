#include "interpreter.h"
#include <readline/readline.h>

/*
	TODO:
		- Fix the fact that you have to not create files upon denied permission.
		- Finish the heredoc.
*/

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
		close(STDIN_FILENO);
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
	{ // stop nearest word from executing.
		if (closest_word)
			closest_word->data.command.execute = false;
		return (perror(rredir->data.redirection.filename),
			mshcontext->permissions_clear = false, EXIT_FAILURE);
	}
	close(STDOUT_FILENO);
	mode = O_TRUNC * (mode != O_APPEND) | mode;
	fd = open(rredir->data.redirection.filename, O_CREAT | O_WRONLY | mode, 0755);
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
	t_astnode	*closest_word;
	char		*buffer;
	char		*input;
	char		*temp;
	int			pipedes[2];

	input = NULL;
	buffer = (char *)1;
	closest_word = heredoc->parent;
	if (closest_word)
		while (closest_word->parent && closest_word->type != TK_WORD)
			closest_word = closest_word->parent;
	while (buffer)
	{
		buffer = readline("> ");
		if (!buffer)
			return (/*free stuff, */ EXIT_FATAL);
		if ((!ft_strncmp(buffer, heredoc->data.redirection.filename, ft_strlen(heredoc->data.redirection.filename) + 1)
			|| *buffer == EOF)
			&& (free(buffer), 1))
			break ;
		temp = buffer;
		buffer = ft_strjoin(buffer, "\n");
		if (!buffer)
			return (free(temp), mshcontext->terminate = true, EXIT_FATAL);
		free(temp);
		temp = input;
		input = ft_strjoin(input, buffer);
		if (!input)
			return (free(temp), free(buffer), mshcontext->terminate = true, EXIT_FATAL);
		(free(temp), free(buffer));
		// rl_on_new_line();
	}
	if (pipe(pipedes) < 0)
		return (free(input), mshcontext->terminate = true, EXIT_FATAL);
	ft_putstr_fd(input, pipedes[WRITE_END]);
	close(pipedes[WRITE_END]);
	if (mshcontext->permissions_clear)
		(dup2(pipedes[READ_END], STDIN_FILENO));
	if (closest_word)
		closest_word->data.command.thereisin = true;
	close(pipedes[READ_END]);
	free(input);
	return (EXIT_SUCCESS);
}
