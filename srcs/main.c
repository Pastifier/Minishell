#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_node	*envl;

	(void)argc; (void)argv; (void)envp;
	envl = NULL;
	str_arr_dup_to_list(envp, &envl);
	export(&envl, "fate=", "Inception");
	export(&envl, "falafel=", "falafel");
	export(&envl, "falafel=", "weed");
	unset(&envl, "fate=");
	env(&envl);
	list_destroy(&envl);
	return (0);
}
