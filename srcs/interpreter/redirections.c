/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 18:53:27 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/18 19:32:23 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"
#include <stdio.h>

static int	check_lredir_and_act_accordingly(t_astnode *lredir,
				t_shcontext *mshcontext);
static int	check_rredir_and_act_accordingly(t_astnode *rredir,
				t_shcontext *mshcontext);

int	handle_lredir(t_astnode *lredir, t_shcontext *mshcontext)
{
	if (lredir->type != TK_LREDIR)
		return (EXIT_NEEDED);
	if (lredir->data.redirection.mode == O_APPEND)
		return (handle_heredoc(lredir, mshcontext));
	else if (mshcontext->permissions_clear)
		return (check_lredir_and_act_accordingly(lredir, mshcontext));
	return (EXIT_FAILURE);
}

int	handle_rredir(t_astnode *rredir, t_shcontext *mshcontext)
{
	if (rredir->type != TK_RREDIR)
		return (EXIT_NEEDED);
	if (mshcontext->permissions_clear)
		return (check_rredir_and_act_accordingly(rredir, mshcontext));
	return (EXIT_FAILURE);
}

int	handle_heredoc(t_astnode *heredoc, t_shcontext *mshcontext)
{
	t_astnode	*closest_word;

	(void)mshcontext;
	closest_word = heredoc->parent;
	if (closest_word)
		while (closest_word->parent && closest_word->type != TK_WORD)
			closest_word = closest_word->parent;
	if (mshcontext->permissions_clear)
	{
		close(STDIN_FILENO);
		dup2(heredoc->data.redirection.fd[READ_END], STDIN_FILENO);
		if (closest_word)
			closest_word->data.command.thereisin = true;
	}
	close(heredoc->data.redirection.fd[READ_END]);
	return (EXIT_SUCCESS);
}

static int	check_lredir_and_act_accordingly(t_astnode *lredir,
		t_shcontext *mshcontext)
{
	int			fd;
	t_astnode	*closest_word;

	closest_word = lredir->parent;
	if (closest_word)
		while (closest_word->parent && closest_word->type != TK_WORD)
			closest_word = closest_word->parent;
	if (access(lredir->data.redirection.filename, F_OK)
		|| access(lredir->data.redirection.filename, R_OK))
	{
		if (closest_word)
			closest_word->data.command.execute = false;
		return (perror(lredir->data.redirection.filename),
			mshcontext->permissions_clear = false, EXIT_FAILURE);
	}
	close(STDIN_FILENO);
	fd = open(lredir->data.redirection.filename, O_RDONLY, 0755);
	if (fd < 0)
		return (EXIT_FATAL);
	if (closest_word)
		closest_word->data.command.thereisin = true;
	return (EXIT_SUCCESS);
}

static int	check_rredir_and_act_accordingly(t_astnode *rredir,
		t_shcontext *mshcontext)
{
	int			fd;
	int			mode;
	t_astnode	*closest_word;

	closest_word = rredir->parent;
	if (closest_word)
		while (closest_word->parent && closest_word->type != TK_WORD)
			closest_word = closest_word->parent;
	if (!access(rredir->data.redirection.filename, F_OK)
		&& access(rredir->data.redirection.filename, W_OK))
	{
		if (closest_word)
			closest_word->data.command.execute = false;
		return (perror(rredir->data.redirection.filename),
			mshcontext->permissions_clear = false, EXIT_FAILURE);
	}
	close(STDOUT_FILENO);
	mode = O_TRUNC * (rredir->data.redirection.mode != O_APPEND)
		| rredir->data.redirection.mode | O_WRONLY | O_CREAT;
	fd = open(rredir->data.redirection.filename, mode, 0755);
	if (fd < 0)
		return (EXIT_FATAL);
	if (closest_word)
		closest_word->data.command.thereisout = true;
	return (EXIT_SUCCESS);
}
