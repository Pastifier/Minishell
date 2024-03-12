# include "minishell.h"

void pwd(void)
{
    char *path;

    path = getcwd(NULL, 0);
    ft_putendl_fd(path, 1);
    free(path);
}