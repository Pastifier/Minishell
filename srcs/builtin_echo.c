# include "minishell.h"

/*
git checkout ahmed
git push origin ahmed
git submodule update
echo builtin implementation:
needed output:
    - echo -n "string" -> no newline
    - echo "string" -> newline
    - echo "string" "$?" -> print exit status
    - echo "string" "$var" -> print env value
needed input:
    - char **command -> array of strings
    - command[0] -> "echo"
    - command[1] -> "-n" or "string" or "$?" or "$var"
    - command[2] -> "string" or "$?" or "$var"
    - ...
    - command[n] -> NULL
test cases:
    - echo "string" -> string\n
    - echo -n "string" -> string
*/
// void echo(char **command)
// {
//     int i;
//     int n_flag;

//     i = 1;
//     n_flag = 0;
//     if (command[i] && ft_strcmp(command[i], "-n") == 0)
//         n_flag = 1;
//     while (command[++i])
//     {
//         if (command[i][0] == '$')
//         {
//             if (ft_strcmp(command[i], "$?") == 0)
//                 ft_putstr_fd(exit_status, 1); // need to implement add exit status to the stuct
//             else
//                 ft_putstr_fd(get_env_value(command[i] + 1), 1);
//         }
//         else
//             ft_putstr_fd(command[i], 1);
//         if (command[i + 1])
//             ft_putstr_fd(" ", 1);
//     }
//     if (!n_flag)
//         ft_putstr_fd("\n", 1);
// }

void echo(char *str, int n_flag)
{
    if (n_flag)
        ft_putstr_fd(str, 1);
    else
        ft_putendl_fd(str, 1);
}

