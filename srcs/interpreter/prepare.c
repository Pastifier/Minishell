/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 01:27:14 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/16 04:30:31 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"

int	prepare_pipenode(t_astnode *pipenode)
{
	t_astnode *left_child;

	if (pipenode->type != TK_PIPE)
		return (EXIT_NEEDED);
	left_child = pipenode->left;
	if (left_child->type == TK_WORD)
	{
		if (pipe(left_child->data.command.fd) < 0)
			return (EXIT_FATAL);
		left_child->data.command.thereispipe = true;
	}
	else if (left_child->type == TK_PIPE)
	{
		if (pipe(left_child->right->data.command.fd) < 0)
			return (EXIT_FATAL);
		left_child->right->data.command.thereispipe = true;
	}
	return (EXIT_SUCCESS);
}
