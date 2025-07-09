/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:15:06 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/08 18:51:46 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
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

char	*join_strings(char *s1, char *s2)
{
	char	*ret;

	ret = ft_strjoin(s1, s2);
	if (s1)
		free(s1);
	if (!ret)
		return (NULL);
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
	cmp = ft_strncmp(*to_write, "-n", 2);
	i = 0;
	while (!cmp && to_write[++i])
		cmp = ft_strncmp(to_write[i], "-n", 2);
	while (to_write[i])
	{
		out = join_strings(out, to_write[i]);
		if (out && to_write[i + 1])
			out = join_strings(out, " ");
		if (!out)
			return (0);
		i++;
	}
	if (cmp)
		out = join_strings(out, "\n");
	if (!out)
		return (0);
	ft_putstr_fd(out, fd);
	if (out)
		free(out);
	return (1);
}

static void	exit_numeric(t_exec *exec, int	*i)
{
	if (!ft_isdigit(exec->full_cmd[1][*i]) && exec->full_cmd[1][*i] != '+'
			&& exec->full_cmd[1][*i] != '-')
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(exec->full_cmd[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	(*i)++;
}

void	bi_exit(t_exec **exec, char ***envp, t_history *hist)
{
	int	ret;
	int	i;

	ret = 1;
	printf("exit\n");
	if ((*exec)->full_cmd[1] && (*exec)->full_cmd[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return ;
	}
	if ((*exec)->full_cmd[1])
	{
		i = 0;
		while ((*exec)->full_cmd[1][i])
			exit_numeric((*exec), &i);
		if (i > 0 && !(*exec)->full_cmd[1][i])
			ret = ft_atoi((*exec)->full_cmd[1]);
	}
	free_array(*envp);
	burn_history(hist);
	lynch_exec(exec);
	exit(ret);
}
