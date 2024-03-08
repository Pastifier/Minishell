#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc; (void)argv; (void)envp;
	write(STDOUT_FILENO, "Hello\n", 6);
	return (0);
}
