/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dump.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:48:42 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/21 00:12:34 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

int	confirm_var_visibility(size_t val_length, t_node *varp, void *content)
{
	if (val_length)
		varp->visible = true;
	if (varp->content)
		free(varp->content);
	varp->content = content;
	return (EXIT_SUCCESS);
}

int	check_home_and_cd_if_it_exists(t_node **envp)
{
	t_node	*home;
	char	*eql_addr;

	home = find_variable(envp, "HOME=");
	if (!home)
		return (ft_putendl_fd(NO_HOME, STDERR_FILENO), EXIT_FAILURE);
	eql_addr = ft_strchr(home->content, '=');
	if (!eql_addr || (eql_addr && !*(eql_addr + 1)))
		return (ft_putendl_fd(NO_HOME, STDERR_FILENO), EXIT_FAILURE);
	return (cd(eql_addr + 1, envp));
}

int	check_oldpwd_and_cd_if_it_exists(t_node **envp)
{
	t_node	*oldpwd;
	char	*eql_addr;

	oldpwd = find_variable(envp, "OLDPWD=");
	if (!oldpwd)
		return (ft_putendl_fd(NO_OLDPWD, STDERR_FILENO), EXIT_FAILURE);
	eql_addr = ft_strchr(oldpwd->content, '=');
	if (!eql_addr || (eql_addr && !*(eql_addr + 1)))
		return (ft_putendl_fd(NO_OLDPWD, STDERR_FILENO), EXIT_FAILURE);
	return (cd(eql_addr + 1, envp));
}
