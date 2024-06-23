/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper_dump.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:10:40 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/23 21:38:18 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

int	handle_slash_is_dir(char **args, DIR *dir)
{
	write(STDERR_FILENO, "msh: ", 6);
	ft_putstr_fd(args[0], STDERR_FILENO);
	write(STDERR_FILENO, ": is a directory\n", 18);
	(str_arr_destroy(args), closedir(dir));
	return (126);
}

int	handle_slash_not_dir(char **args)
{
	write(STDERR_FILENO, "msh: ", 6);
	ft_putstr_fd(args[0], STDERR_FILENO);
	write(STDERR_FILENO, ": No such file or directory\n", 28);
	(str_arr_destroy(args));
	return (127);
}

int	handle_no_pathnode(char **args, char **envp)
{
	execve(args[0], args, envp);
	write(STDERR_FILENO, "msh: ", 6);
	ft_putstr_fd(args[0], STDERR_FILENO);
	write(STDERR_FILENO, ": command not found\n", 20);
	(str_arr_destroy(args));
	return (127);
}
