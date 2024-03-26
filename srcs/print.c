# include "../include/minishell.h"

// print_array function
void print_array(char **array)
{
	int i;

	i = 0;
    printf("array: ");
	while (array[i])
	{
		printf("%s, ", array[i]);
		i++;
	}
}

void print_tokens(t_token **token)
{
    t_token *iter;

    iter = *token;
    while (iter)
    {
        printf("token: %s type: %u\n", iter->value, iter->token_type);
        iter = iter->next;
    }
}
