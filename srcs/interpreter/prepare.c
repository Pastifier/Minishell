/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalshafy <aalshafy@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 01:27:14 by ebinjama          #+#    #+#             */
/*   Updated: 2024/04/23 16:41:16 by aalshafy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"
#include <stdio.h>

int	prepare_pipenode(t_astnode *pipenode)
{
	t_astnode	*left_child;
	t_astnode	*closest_left;

	pipenode->data.pipe.thereisinput = false;
	if (pipenode->type != TK_PIPE)
		return (EXIT_NEEDED);
	left_child = pipenode->left;
	if (left_child->type == TK_WORD)
	{
		if (pipe(left_child->data.command.fd) < 0)
			return (perror("pipe()"), EXIT_FATAL);
		left_child->data.command.thereispipe = true;
		pipenode->right->data.command.thereisprev = true;
		pipenode->right->data.command.prevfd = left_child->data.command.fd;
		pipenode->data.pipe.thereisinput = true;
	}
	else if (left_child->type == TK_PIPE)
	{
		closest_left = left_child->right;
		if (pipe(closest_left->data.command.fd) < 0)
			return (perror("pipe()"), EXIT_FATAL);
		closest_left->data.command.thereispipe = true;
		pipenode->right->data.command.thereisprev = true;
		pipenode->right->data.command.prevfd = closest_left->data.command.fd;
		pipenode->data.pipe.thereisinput = true;
	}
	return (EXIT_SUCCESS);
}

int	prepare_rredir(t_astnode *rredir)
{
	t_astnode	*concerned_node;
	int			*fd;

	if (rredir->type != TK_RREDIR)
		return (EXIT_NEEDED);
	fd = &rredir->data.redirection.fd;
	*fd = open(rredir->data.redirection.filename, O_CREAT | O_WRONLY);
	if (*fd < 0)
		return (EXIT_FATAL);
	concerned_node = rredir->left;
	if (concerned_node->type == TK_PIPE)
		concerned_node = concerned_node->right;
	concerned_node->data.command.thereisout = true;
	concerned_node->data.command.outfd = *fd;
	return (EXIT_SUCCESS);
}


/*
			  (PIPE)
		(PIPE)		CMD4
	(PIPE)	  CMD3
CMD1	  CMD2

*/
