/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 23:40:22 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/09 13:45:33 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"
#include <stdio.h>

extern int	g_signal;

int	handle_pipe(t_astnode *pipenode, t_shcontext *mshcontext)
{
	if (pipenode->type == TK_PIPE)
	{
		dup2(mshcontext->stds[0], STDIN_FILENO);
		dup2(mshcontext->stds[1], STDOUT_FILENO);
		close(mshcontext->stds[0]);
		close(mshcontext->stds[1]);
		mshcontext->stds[0] = dup(STDIN_FILENO);
		mshcontext->stds[1] = dup(STDOUT_FILENO);
		mshcontext->permissions_clear = true;
		if (pipenode->left && !pipenode->right)
			close(pipenode->left->data.command.fd[READ_END]);
	}
	return (0);
}
