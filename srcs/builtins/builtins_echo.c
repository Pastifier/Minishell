#include "minishell.h"

int	echo(t_astnode *word, t_node *first_arg)
{
	bool	newline;
	t_node	*iter;

	(void)word;
	iter = first_arg;
	newline = true;
	if (first_arg && !ft_strncmp(first_arg->content, "-n", 2))
	{
		newline = false;
		iter = first_arg->next;
	}
	while (iter)
	{
		ft_putstr_fd(iter->content, STDOUT_FILENO);
		if (iter->next)
			ft_putchar_fd(' ', STDOUT_FILENO);
		iter = iter->next;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
