/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 17:21:21 by ebinjama          #+#    #+#             */
/*   Updated: 2024/03/12 14:36:06 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(t_node **envp)
{
	t_node	*iter;
	if (!envp)
		return ;
	iter = *envp;
	while (iter)
	{
		printf("%s\n", iter->content);
		iter = iter->next;
	}
}