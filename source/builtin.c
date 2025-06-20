/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:15:06 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/09 13:26:30 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <limits.h>
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
		return (0);
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
	cmp = ft_strncmp(*to_write, "-n", 3);
	i = 0;
	if (!cmp)
		i = 1;
	while (to_write[i])
	{
		out = join_strings(out, to_write[i]);
		if (to_write[i + 1])
			out = join_strings(out, " ");
		i++;
	}
	if (cmp)
		out = join_strings(out, "\n");
	ft_putstr_fd(out, fd);
	if (out)
		free(out);
	return (1);
}

int	cd(char **path, char ***env)
{
	char	*cwd;
	char	*joined_path;

	if (!path || !path[1])
		return (0);
	if (path[2])
		return (ft_putstr_fd(" too many arguments\n", 2), 0);
	(void)env;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	if (ft_strncmp(cwd, path[1], ft_strlen(cwd)) && path[1][0] != '/')
	{
		joined_path = ft_strjoin(cwd, "/");
		if (cwd)
			free(cwd);
		joined_path = ft_strjoin(joined_path, path[1]);
		if (chdir(joined_path) == -1)
			return (ft_putstr_fd(" No such file or directory\n", 2), free(joined_path), 0);
	}
	else
	{
		if (chdir(path[1]) == -1)
			return (ft_putstr_fd(" No such file or directory\n", 2), free(cwd), 0);
	}
	return (1);
}

void	bi_exit(t_exec *exec, int child)
{
	int	ret;
	int	i;

	if (child)
		return ;
	ret = 1;
	if (exec->full_cmd[2])
	{
		ft_putstr_fd(" too many arguments\n", 2);
		exit (1);
	}
	if (exec->full_cmd[1])
	{
		i = 0;
		while (exec->full_cmd[1][i])
		{
			if (!ft_isdigit(exec->full_cmd[1][i]) && exec->full_cmd[1][i] != '+' && exec->full_cmd[1][i] != '-')
			{
				ft_putstr_fd(" numeric argument required\n", 2);
				exit(2);
			}
			i++;
		}
		if (i > 0 && !exec->full_cmd[1][i])
			ret = ft_atoi(exec->full_cmd[1]);
	}
	printf("exit\n");
	exit(ret);
}
