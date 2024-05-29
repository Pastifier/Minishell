/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 01:27:14 by ebinjama          #+#    #+#             */
/*   Updated: 2024/05/29 12:28:46 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"
#include <stdio.h>

int	prepare_pipenode(t_astnode *pipenode, t_shcontext *mshcontext)
{
	t_astnode	*left_child;
	t_astnode	*closest_left;

	if (pipenode->type != TK_PIPE/* || mshcontext->terminate*/)
		return (EXIT_NEEDED);
	left_child = pipenode->left;
	if (left_child->type == TK_WORD)
	{
		if (pipe(left_child->data.command.fd) < 0)
			return (perror("pipe()"), mshcontext->terminate = true, EXIT_FATAL);
		left_child->data.command.thereispipe = true;
		if (pipenode->right)
		{
			pipenode->right->data.command.thereisprev = true;
			pipenode->right->data.command.prevfd = left_child->data.command.fd;
			pipenode->data.pipe.thereisinput = true;
		}
	}
	else if (left_child->type == TK_PIPE)
	{
		closest_left = left_child->right;
		if (pipe(closest_left->data.command.fd) < 0)
			return (perror("pipe()"), mshcontext->terminate = true, EXIT_FATAL);
		closest_left->data.command.thereispipe = true;
		if (pipenode->right)
		{
			pipenode->right->data.command.thereisprev = true;
			pipenode->right->data.command.prevfd = closest_left->data.command.fd;
			pipenode->data.pipe.thereisinput = true;
		}
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
	unlink(rredir->data.redirection.filename);
	*fd = open(rredir->data.redirection.filename, O_CREAT | O_WRONLY, 0755);
	if (*fd < 0)
		return (EXIT_FATAL);
	// it executes the word normally if this doesn't exist. (it's a fatal error. It should stop).
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
