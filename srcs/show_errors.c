/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:22:25 by aalshafy          #+#    #+#             */
/*   Updated: 2024/06/26 15:12:14 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_syntax_error(char *token)
{
	char	*msg;
	char	*temp;

	msg = ft_strjoin("minishell: syntax error near unexpected token `", token);
	if (!msg)
	{
		show_error(1);
		return ;
	}
	temp = msg;
	msg = ft_strjoin(msg, "'\n");
	free(temp);
	if (!msg)
	{
		show_error(1);
		return ;
	}
	write(2, msg, ft_strlen(msg));
	free(msg);
}

void	show_error(int exit_status)
{
	char	*msg;

	msg = NULL;
	if (exit_status == 0)
		return ;
	if (exit_status == 1)
		msg = "Error: malloc failed\n";
	else if (exit_status == 2)
		return ;
	else if (exit_status == 3)
		msg = "Error: unclosed quotes\n";
	else if (exit_status == 4)
		return ;
	else if (exit_status == 5)
		return ;
	else if (exit_status == 6)
		return ;
	if (msg)
		write(2, msg, ft_strlen(msg));
}
