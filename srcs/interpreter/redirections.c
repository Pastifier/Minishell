#include "minishell.h"
#include <stdlib.h>

int handle_lredir(t_astnode *lredir)
{
  int fd;

  if (lredir->type != TK_LREDIR)
    return (EXIT_NEEDED);
  if (!access(lredir->data.redirection.filename, F_OK))
    unlink(lredir->data.redirection.filename);
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

  if (rredir->type != TK_RREDIR)
    return (EXIT_NEEDED);
  if (!access(rredir->data.redirection.filename, F_OK))
    unlink(rredir->data.redirection.filename);
  close(STDOUT_FILENO);
  fd = open(rredir->data.redirection.filename, O_CREAT | O_WRONLY, 0755);
  if (fd < 0)
    return (EXIT_FATAL);
  rredir->data.redirection.fd = fd;
  return (EXIT_SUCCESS);
}
