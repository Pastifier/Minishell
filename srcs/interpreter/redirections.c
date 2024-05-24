#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

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

int handle_rredir(t_astnode *rredir)
{
	int fd;
	t_astnode *closest_word;

	if (rredir->type != TK_RREDIR)
		return (EXIT_NEEDED);
	closest_word = rredir->parent;
	if (access(rredir->data.redirection.filename, W_OK))
	{	// stop nearest word from executing.
		while (closest_word && closest_word->type != TK_WORD)
			closest_word = closest_word->parent;
		closest_word->data.command.execute = false;
		return (perror(rredir->data.redirection.filename), EXIT_NEEDED);
	}
	if (!access(rredir->data.redirection.filename, F_OK))
		unlink(rredir->data.redirection.filename);
	close(STDOUT_FILENO);
	fd = open(rredir->data.redirection.filename, O_CREAT | O_WRONLY, 0755);
	if (fd < 0)
		return (EXIT_FATAL);
	rredir->data.redirection.fd = fd;
	return (EXIT_SUCCESS);
}
