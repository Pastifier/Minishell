#include "minishell.h"
#include <stdio.h>

int pwd(void)
{
    char *path;

    path = getcwd(NULL, 0);
    if (!path)
    {
        perror("pwd"); // permission denied or file not found or resoure busy (malloc failled)
        return (EXIT_FAILURE);
    }
    ft_putendl_fd(path, 1);
    free(path);
    return (EXIT_SUCCESS);
}
