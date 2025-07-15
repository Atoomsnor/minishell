/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:15:06 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/15 19:08:44 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <limits.h>
#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	pwd(int fd)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	else if (fd < 0)
		return (free(cwd), 0);
	else
		ft_putendl_fd(cwd, fd);
	if (cwd)
		free(cwd);
	return (1);
}

static int	check_flag(char **to_write, int *i)
{
	int	j;
	int	ret;

	ret = 0;
	while (to_write[*i])
	{
		if (to_write[*i][0] != '-')
			break ;
		j = 1;
		while (to_write[*i][j])
		{
			if (to_write[*i][j] != 'n')
				return (ret);
			j++;
		}
		ret = 1;
		(*i)++;
	}
	return (ret);
}

int	echo(int fd, char **to_write)
{
	char	*out;
	int		cmp;
	int		i;

	if (!to_write || !*to_write || fd < 0)
		return (0);
	out = NULL;
	i = 0;
	cmp = check_flag(to_write, &i);
	while (to_write[i])
	{
		out = ft_strjoin_free(out, to_write[i], 1);
		if (out && to_write[i + 1])
			out = ft_strjoin_free(out, " ", 1);
		if (!out)
			return (0);
		i++;
	}
	if (!cmp)
		out = ft_strjoin_free(out, "\n", 1);
	if (!out)
		return (0);
	ft_putstr_fd(out, fd);
	free_and_null(out);
	return (1);
}

static void	exit_numeric(t_exec **exec, int	*i, char ***envp, t_history *hist)
{
	if (!ft_isdigit((*exec)->full_cmd[1][*i]) && (*exec)->full_cmd[1][*i] != '+'
			&& (*exec)->full_cmd[1][*i] != '-')
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd((*exec)->full_cmd[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_array(*envp);
		burn_history(hist);
		lynch_exec(exec);
		exit(2);
	}
	(*i)++;
}

int	bi_exit(t_exec **exec, char ***envp, t_history *hist)
{
	unsigned long	ret;
	int				i;

	ret = 1;
	printf("exit\n");
	
	if ((*exec)->full_cmd[1])
	{
		i = 0;
		while ((*exec)->full_cmd[1][i])
			exit_numeric(exec, &i, envp, hist);
		if (i > 0 && !(*exec)->full_cmd[1][i])
			ret = ft_atoul((*exec)->full_cmd[1]);
		if (ret > LONG_MAX)
			exit_numeric(exec, &i, envp, hist);
	}
	if ((*exec)->full_cmd[1] && (*exec)->full_cmd[2])
			return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	free_array(*envp);
	burn_history(hist);
	lynch_exec(exec);
	exit(ret);
	return (0);
}
