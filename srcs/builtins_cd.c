# include "minishell.h"

void cd(char *path)
{
    if (chdir(path) == -1)
    {
        ft_putstr_fd("cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": ", 2);
        ft_putendl_fd(strerror(errno), 2);
    }
}