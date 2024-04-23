#include "minishell.h"
#include "parser.h"

char **get_command_args(t_token **token)
{
    char **args;
    t_token *temp;
    int i;

    i = 0;
    temp = *token;
    while (temp && temp->token_type == TK_WORD)
    {
        i++;
        temp = temp->next;
    }
	args = ft_calloc(i + 1, sizeof(char *));
    if (!args)
        return (NULL);
    temp = *token;
    i = 0;
    while (temp && temp->token_type == TK_WORD)
    {
        args[i] = ft_strdup(temp->value);
        if (!args[i])
            return (destroy_str_arr(args), NULL);
        i++;
        temp = temp->next;
    }
    args[i] = NULL;
    (*token)->next = temp;
    return (args);
}

char *ft_str_toupper(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        str[i] = ft_toupper(str[i]);
        i++;
    }
    return (str);
}

t_cid get_builtin_id(t_token **token)
{
    char *command;

    command = (*token)->value;
    if (ft_strncmp(command, "cd", 2) == 0) // need to check for ft_strncmp
        return (CD);
    else if (ft_strncmp(command, "echo", 4) == 0)
        return (ECHO);
    else if (ft_strncmp(command, "pwd", 3) == 0)
        return (PWD);
    else if (ft_strncmp(command, "env", 3) == 0)
        return (ENV);
    else if (ft_strncmp(command, "export", 6) == 0)
        return (EXPORT);
    else if (ft_strncmp(command, "unset", 5) == 0)
        return (UNSET);
    else if (ft_strncmp(command, "exit", 4) == 0)
        return (EXIT);
    else
        return (0);
}

