/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 10:09:10 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/27 16:40:46 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include <stdio.h>

static int	set_env(char *name, char *value, t_node **envp);
static int	add_env(char *name, char *value, t_node **envp);
int			cd(char *path, t_node **envp);

int	wcd(t_astnode *cdnode, t_shcontext *mshcontext)
{
	t_node	*cdarg;
	char	first_char_of_cdarg;
	bool	there_is_no_more;

	cdarg = cdnode->data.command.args->next;
	if (cdarg)
	{
		first_char_of_cdarg = *(char *)cdarg->content;
		there_is_no_more = !*((char *)cdarg->content + 1);
		if (!cdarg->next && !OS_IS_MAC)
			if (first_char_of_cdarg == '-' && there_is_no_more)
				return (check_oldpwd_and_cd_if_it_exists(&mshcontext->envl));
	}
	if (cdarg && OS_IS_MAC)
		return (cd(cdarg->content, &(mshcontext->envl)));
	if (cdarg && cdarg->next)
	{
		ft_putendl_fd("msh: cd: too many arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else if (cdarg)
		return (cd(cdarg->content, &(mshcontext->envl)));
	return (cd(NULL, &(mshcontext->envl)));
}

int	cd(char *path, t_node **envp)
{
	char	*oldpwd;
	char	*pwd;

	if (!path)
		return (check_home_and_cd_if_it_exists(envp));
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("msh: cd: ", STDERR_FILENO);
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

static int	set_env(char *name, char *value, t_node **envp)
{
	t_node	*node;
	char	*env;
	char	*tmp;

	node = *envp;
	node = node->next;
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
			free(tmp);
			return (EXIT_SUCCESS);
		}
		node = node->next;
	}
	return (add_env(name, value, envp));
}

static int	add_env(char *name, char *value, t_node **envp)
{
	char	*env;
	char	*tmp;
	t_node	*node;

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
