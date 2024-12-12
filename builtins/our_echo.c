#include "../minishell.h"

// checks if the argument is only n
bool check_non_n(char *arg)
{
    int i;

    i = 1;
    while(arg[i])
    {
        if(arg[i] != 'n')
            return (false);
        i++;
    }
    return (true);
}

//need to deal with the quotes still

// change it to int and return 0
void our_echo(char **arg, t_shell *data)
{
    int flag_n;
    int flag_f;
    int i;
    char *echo_arg;

    i = 1;
    flag_f = 0;
    flag_n = 1;
    if(!arg[i])
        write(1, " ", 1);
    else
    {
        if(ft_strncmp(arg[i], "-n", 2) == 0 && check_non_n(arg[i])) // added the check_non_n function
        {
            flag_n = 0;
            i++;
        }
        while(arg[i])
        {
            echo_arg = arg[i];
            if (arg[i][0] == '$')
            {
                flag_f = 1;
                echo_arg = our_expand(arg[i] + 1, data);
                if (!echo_arg)
                    printf("malloc fail\n"); //malloc fail check
            }
            write(1, echo_arg, ft_strlen(echo_arg));
            if((arg[i + 1]))
                write(1, " ", 1);
            i++;
            if (flag_f)
            {
                free(echo_arg);
                flag_f = 0;
            }
        }
    }
    if(flag_n)
        write(1, "\n", 1);
}
