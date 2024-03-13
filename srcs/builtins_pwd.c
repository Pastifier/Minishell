#include "minishell.h"

int pwd(void)
{
    char *path;

    path = getcwd(NULL, 0);
    if (!path)
    {
        ft_putendl_fd("pwd: getcwd error", 2); // permission denied or file not found or resoure busy (malloc failled)
        return (EXIT_FAILURE);
    }
    ft_putendl_fd(path, 1);
    free(path);
    return (EXIT_SUCCESS);
}