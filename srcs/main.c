#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

int	main(int argc, char **argv, char **envp)
{
	t_node	*envl;

	(void)argc; (void)argv;
	envl = NULL;
	str_arr_dup_to_list(envp, &envl); // Add check and exit fatally if it fails (it returns a bool)
	list_destroy(&envl);
	return (0);
}
