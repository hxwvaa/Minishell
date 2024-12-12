#include "../minishell.h"

int	is_digit_exit_code(char **av)
{
	int	n;

	n = 0;
	if (av[1][n] == '-' || av[1][n] == '+')
		n++;
	while (av[1][n])
	{
		if (ft_isdigit(av[1][n]))
			;
		else
			return (0);
		n++;
	}
	return (1);
}

bool	is_num_very_small(char *str)
{
	size_t		i;
	long long	nb;

	i = 0;
	nb = 0;
	if (ft_atol(str) == LONG_MIN)
		return (false);
	if (str[i] == '-')
		i++;
	if (ft_strlen(str) > 19)
		return (true);
	else if (ft_strlen(str) < 19)
		return (false);
	while (i < ft_strlen(str) && str[i] >= '0' && str[i] <= '9')
		nb = nb * 10 + str[i++] - '0';
	if (nb * -1 < LONG_MIN)
		return (true);
	i = str[i] - '0';
	if (i > 8)
		return (true);
	return (false);
}

void	exit_av_more_than_2(char **av, t_shell *data)
{
	if (is_digit_exit_code(av))
	{
		(printf("exit\n"), printf("shell: exit: too many arguments\n"));
		data->exit_code = 1;
		return ;
	}
	else
	{
		printf("exit\n");
		printf("shell: exit: %s: numeric argument required\n", av[1]);
		free_all(data);
		exit(255);
	}
}

void	exit_av_is_equal_2(char **av, t_shell *data)
{
	bool				b;
	unsigned long long	n;

	b = false;
	n = 0;
	if (is_digit_exit_code(av))
	{
		if (av[1][0] == '-')
			b = is_num_very_small(av[1]);
		if (b == false)
		{
			if (av[1][0] != '-')
				n = ft_atoull(av[1]);
			if ((n <= LONG_MAX && b == false) || (av[1][0] == '-'
					&& b == false))
			{
				n = ft_atol(av[1]);
				printf("exit\n");
				(free_all(data), exit(n % 256));
			}
		}
	}
	printf("exit\nshell: exit: %s: numeric argument required\n", av[1]);
	free_all(data);
	exit(255);
}

void	exit_shell(char **av, t_shell *data)
{
	int	i;

	i = 0;
	while (av[i])
		i++;
	if(data->std[0] != -1)
		close(data->std[0]);
	if(data->std[1] != -1)
		close(data->std[1]);//maybe add checks or something // now no fd leaks
	if(data->fd[0] != -1) // without this 1 fd leak in case of exit | pwd
		close(data->fd[0]);
	if (i == 1)
		(write(2,"exit\n", 5),free(data->backup_pwd), free_arr(data->envi), our_toklistclear(&data->tokens), our_envlistclear(&data->envir),  our_cmdlistclear(&data->cmds), exit(0));
	//(printf("exit\n"), free_arr(av), our_toklistclear(&data->tokens), our_envlistclear(&data->envir),  our_cmdlistclear(&data->cmds), exit(0));  
	else if (i == 2)
		exit_av_is_equal_2(av, data);
	else
		exit_av_more_than_2(av, data);
}
