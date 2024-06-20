/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dump.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:48:42 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/20 17:14:53 by ebinjama         ###   ########.fr       */
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
