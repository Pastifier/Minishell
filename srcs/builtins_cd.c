#include "minishell.h"

int cd(char *path, char **envp)
{
    char *oldpwd;
    char *pwd;

    oldpwd = getcwd(NULL, 0);
    if (chdir(path) == -1)
    {
        ft_putstr_fd("cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": ", 2);
        ft_putendl_fd("No such file or directory", 2);
        free(oldpwd);
        return (EXIT_FAILURE);
    }
    else
    {
        pwd = getcwd(NULL, 0);
        set_env("OLDPWD", oldpwd, envp);
        set_env("PWD", pwd, envp);
        free(oldpwd);
        free(pwd);
    }
    return (EXIT_SUCCESS);
}
static int set_env(char *name, char *value, t_node **envp)
{
    t_node *node;
    char *env;
    char *tmp;

    node = *envp;
    while (node)
    {
        env = (char *)node->content;
        if (ft_strncmp(env, name, ft_strlen(name)) == 0)
        {
            free(env);
            tmp = ft_strjoin(name, "=");
            if (!tmp)
                return (EXIT_FAILURE);
            node->content = ft_strjoin(tmp, value);
            if (!node->content)
                return (free(tmp), EXIT_FAILURE);
            return (EXIT_SUCCESS);
        }
        node = node->next;
    }
    return (add_env(name, value, envp));
}

static int add_env(char *name, char *value, t_node **envp)
{
    char *env;
    char *tmp;
    t_node *node;

    env = ft_strjoin(name, "=");
    if (!env)
        return (EXIT_FAILURE);
    tmp = ft_strjoin(env, value);
    if (!tmp)
        return (free(env), EXIT_FAILURE);
    node = node_create(tmp);
    if (!node)
        return (free(env), free(tmp), EXIT_FAILURE);
    list_append(envp, node);
    return (free(env), free(tmp), EXIT_SUCCESS);
}
