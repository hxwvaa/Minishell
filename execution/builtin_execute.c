/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_execute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 10:19:47 by mshaheen          #+#    #+#             */
/*   Updated: 2024/12/15 18:31:23 by mshaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	only_one_cmd(t_cmd *cmd)
{
	t_cmd	*curr;
	int		i;

	i = 0;
	curr = cmd;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	if (i == 1)
		return (1);
	return (0);
}

void	clean_exit(t_shell *data, int r)
{
	if (data->envir)
		our_envlistclear(&data->envir);
	if (data->tokens)
		our_toklistclear(&data->tokens);
	if (data->cmds)
		our_cmdlistclear(&data->cmds);
	if (data->envi)
		free_arr(data->envi);
	if (data->backup_pwd)
		free(data->backup_pwd);
	if (data->fd[0] != -1)
		close(data->fd[0]);
	data->exit_code = r;
	exit(r);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	else
		return (0);
}

int	execute_one_cmd(t_cmd *curr, t_shell *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (ft_strncmp(curr->cmd, "exit", 5) == 0)
		exit_shell(curr->cargs, data);
	else if (ft_strncmp(curr->cmd, "env", 4) == 0)
		our_env(data->envir);
	else if (ft_strncmp(curr->cmd, "unset", 6) == 0)
		our_unset(curr->cargs[i + 1], &data->envir);
	else if (ft_strncmp(curr->cmd, "echo", 5) == 0)
		our_echo(curr->cargs, data);
	else if (ft_strncmp(curr->cmd, "export", 7) == 0)
		j = our_export(curr->cargs, data, 0);
	else if (ft_strncmp(curr->cmd, "pwd", 4) == 0)
		j = our_pwd(data);
	else if (ft_strncmp(curr->cmd, "cd", 3) == 0)
		j = our_cdir(curr->cargs[i + 1], data);
	data->exit_code = j;
	return (j);
}

void	builtin_pipeline(t_cmd *curr, t_shell *data)
{
	int	r;

	r = execute_one_cmd(curr, data);
	clean_exit(data, r);
}
