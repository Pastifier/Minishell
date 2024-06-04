#include "interpreter.h"
#include <stdio.h>

static int	set_env(char *name, char *value, t_node **envp);
static int	add_env(char *name, char *value, t_node **envp);
static int	cd(char *path, t_node **envp);

int	wcd(t_astnode *cdnode, t_shcontext *mshcontext)
{
	t_node	*cdarg;

	cdarg = cdnode->data.command.args->next;
	if (cdarg)
		return (cd(cdarg->content, &(mshcontext->envl)));
	return (cd(NULL, &(mshcontext->envl)));
}

static int cd(char *path, t_node **envp)
{
    char *oldpwd;
    char *pwd;
    // long arg_limit;

	if (!path)
		return (ft_putendl_fd("msh: invalid use of `cd`. Read the subject, habibi.", STDERR_FILENO), EXIT_NEEDED);
    oldpwd = getcwd(NULL, 0);    
    if (chdir(path) == -1)
    {
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
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
