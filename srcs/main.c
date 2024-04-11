#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_node	*envl;

	(void)argc; (void)argv;
	envl = NULL;
	str_arr_dup_to_list(envp, &envl);
	while (true)
	{
		line = readline("falafel:~$ ");
		ft_putendl_fd("bruh.", STDOUT_FILENO);
		rl_on_new_line();
		// if (!parser())
		//		free(line), continue ;
		// if (!interpreter())
		//		free(line), free(tree), continue ;
		add_history(line);
		free(line);
	}
	list_destroy(&envl);
	return (0);
}
