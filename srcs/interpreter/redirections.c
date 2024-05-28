#include "minishell.h"
#include <stdlib.h>
#include <readline/readline.h>

int handle_lredir(t_astnode *lredir)
{
  int fd;

  if (lredir->type != TK_LREDIR)
    return (EXIT_NEEDED);
  if (access(lredir->data.redirection.filename, F_OK))
    return (perror(lredir->data.redirection.filename), EXIT_NEEDED);
  close(STDIN_FILENO);
  fd = open(lredir->data.redirection.filename, O_CREAT | O_RDONLY, 0755);
  if (fd < 0)
    return (EXIT_FATAL);
  lredir->data.redirection.fd = fd;
  return (EXIT_SUCCESS);
}

int handle_rredir(t_astnode *rredir /*, bool append*/)
{
	int fd;
	t_astnode *closest_word;

	if (rredir->type != TK_RREDIR)
		return (EXIT_NEEDED);
	closest_word = rredir->parent;
	if (!access(rredir->data.redirection.filename, F_OK)
		&& access(rredir->data.redirection.filename, W_OK))
	{	// stop nearest word from executing.
			while (closest_word && closest_word->type != TK_WORD)
				closest_word = closest_word->parent;
			closest_word->data.command.execute = false;
			return (perror(rredir->data.redirection.filename), EXIT_NEEDED);
	}
	close(STDOUT_FILENO);
	fd = open(rredir->data.redirection.filename, O_CREAT | O_WRONLY | O_TRUNC /* | append*/, 0755);
	if (fd < 0)
		return (EXIT_FATAL);
	rredir->data.redirection.fd = fd;
	return (EXIT_SUCCESS);
}

// @brief	This function will keep reading from standard input (you'll need to re-dup stdin back, probably)
//			, and then slaps the result into a pipe. It then dups that pipe into stdin.
int handle_heredoc(t_astnode *heredoc)
{
	char	*buffer;
	char	*input;
	char	*temp;
	int		pipedes[2];

	if (heredoc->type != TK_LAPPEND)
		return (EXIT_NEEDED);
	input = NULL;
	while (buffer)
	{
		buffer = readline("> ");
		if (!buffer)
			return (/*free stuff, */ EXIT_FATAL);
		if (!ft_strncmp(buffer, heredoc->data.heredoc.eof, -1)
			&& (free(buffer), 1))
			break ;
		temp = buffer;
		buffer = ft_strjoin(buffer, "\n");
		if (!buffer)
			return (/*free stuff, */ EXIT_FATAL);
		free(temp);
		temp = input;
		input = ft_strjoin(input, buffer);
		if (!input)
			return (/*free stuff, */ EXIT_FATAL);
		(free(temp), free(buffer));
		rl_on_new_line();
	}
	if (pipe(pipedes) < 0)
		return (/*free stuff, */ EXIT_FATAL);
	ft_putstr_fd(input, pipedes[WRITE_END]);
	close(pipedes[WRITE_END]);
	dup2(pipedes[READ_END], STDIN_FILENO);
	close(pipedes[READ_END]);
	free(input);
	return (EXIT_SUCCESS);
}
