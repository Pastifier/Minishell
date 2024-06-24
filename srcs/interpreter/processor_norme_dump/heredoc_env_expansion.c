/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_env_expansion.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebinjama <ebinjama@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 09:50:31 by ebinjama          #+#    #+#             */
/*   Updated: 2024/06/24 09:50:48 by ebinjama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

void	*ft_realloc(void *ptr, size_t original_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	else if (!ptr)
		return (malloc(new_size));
	else if (new_size <= original_size)
		return (ptr);
	else
	{
		new_ptr = malloc(new_size);
		if (new_ptr)
		{
			ft_memcpy(new_ptr, ptr, original_size);
			free(ptr);
		}
		return (new_ptr);
	}
}

bool	__is_identifier_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

char	*get_env_value(t_shcontext *context, const char *var)
{
	t_node	*current;
	char	*entry;
	char	*equal_sign;
	int		var_len;

	if (*var == '?')
		return (ft_itoa(*(int *)context->envl->content));
	current = context->envl;
	while (current)
	{
		entry = (char *)current->content;
		equal_sign = ft_strchr(entry, '=');
		if (equal_sign)
		{
			var_len = equal_sign - entry;
			if (ft_strncmp(entry, var, var_len) == 0 && var[var_len] == '\0')
				return (ft_strdup(equal_sign + 1));
		}
		current = current->next;
	}
	return (ft_strdup(""));
}

void	handle_dollar(char **original, char **result, t_shcontext *context)
{
	char	*start;
	char	*end;
	char	*var_name;
	char	*value;

	start = *original + 1;
	end = start;
	if (*end == '?' || ft_isdigit(*end))
		end++;
	else
		while (*end && (ft_isalnum(*end) || *end == '_'))
			end++;
	var_name = ft_substr(start, 0, (size_t)(end - start));
	value = get_env_value(context, var_name);
	free(var_name);
	*result = ft_realloc(*result, ft_strlen(*result), ft_strlen(*result)
			+ ft_strlen(value) + 1);
	ft_strlcat(*result, value, ft_strlen(*result) + ft_strlen(value) + 1);
	free(value);
	*original = end;
}

void	expand_env_vars(t_shcontext *context, char **buffer)
{
	char	*result;
	char	*original;
	int		len;

	result = ft_calloc(1, sizeof(char));
	original = *buffer;
	while (*original)
	{
		if (*original == '$' && __is_identifier_char(*(original + 1)))
			handle_dollar(&original, &result, context);
		else
		{
			len = ft_strlen(result);
			result = ft_realloc(result, len, len + 2);
			result[len] = *original;
			result[len + 1] = '\0';
			original++;
		}
	}
	free(*buffer);
	*buffer = result;
}
