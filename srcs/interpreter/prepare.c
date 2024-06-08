/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 01:27:14 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/07 20:35:15 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"
#include <stdio.h>
#include <readline/readline.h>

int	prepare_pipenode(t_astnode *pipenode, t_shcontext *mshcontext)
{
	t_astnode	*left_child;
	t_astnode	*closest_left;

	if (pipenode->type != TK_PIPE/* || mshcontext->terminate*/)
		return (EXIT_NEEDED);
	left_child = pipenode->left;
	if (left_child && left_child->type == TK_WORD)
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
	else if (left_child && left_child->type == TK_PIPE)
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

int	prepare_heredoc(t_astnode *lredir, t_shcontext *mshcontext)
{
	char	*buffer;
	char	*input;
	char	*temp;
	int		*pipedes;

	input = NULL;
	buffer = (char*)1;
	while (buffer)
	{
		buffer = readline("> ");
		if (!buffer)
			break ;
		if (!ft_strncmp(buffer, lredir->data.redirection.filename, -1)
			&& (free(buffer), 1))
			break ;
		temp = buffer;
		buffer = ft_strjoin(buffer, "\n");
		if (!buffer)
			return (free(temp), mshcontext->terminate = true, EXIT_FATAL);
		free(temp);
		temp = input;
		input = ft_strjoin(input, buffer);
		if (!input)
			return (free(temp), free(buffer), mshcontext->terminate = true, EXIT_FATAL);
		(free(temp), free(buffer));
	}
	if (pipe(lredir->data.redirection.fd) < 0)
		return (mshcontext->terminate = true, EXIT_FATAL);
	pipedes = lredir->data.redirection.fd;
	ft_putstr_fd(input, pipedes[WRITE_END]);
	close(pipedes[WRITE_END]);
	free(input);
	return (EXIT_SUCCESS);
}

void	visit_prematurely(t_astnode *node, t_shcontext *mshcontext)
{
	if (!node)
		return ;
	if (node->type == TK_LREDIR && node->data.redirection.mode == O_APPEND)
		prepare_heredoc(node, mshcontext);
	visit_prematurely(node->left, mshcontext);
	visit_prematurely(node->right, mshcontext);
}

/*
			  (PIPE)
		(PIPE)		CMD4
	(PIPE)	  CMD3
CMD1	  CMD2

*/
