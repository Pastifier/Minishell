void echo(char **command)
{
    int i;
    int n_flag;

    i = 1;
    n_flag = 0;
    if (command[i] && ft_strcmp(command[i], "-n") == 0)
    {
        n_flag = 1;
        i++;
    }
    while (command[++i])
    {
        ft_putstr_fd(command[i], 1);
        if (command[i + 1])
            ft_putstr_fd(" ", 1);
    }
    if (!n_flag)
        ft_putstr_fd("\n", 1);

}