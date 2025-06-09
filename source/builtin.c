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

int	cd(char *path)
{
	char	*cwd;
	char	*joined_path;

	if (!path)
		return (0);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	if (ft_strncmp(cwd, path, ft_strlen(cwd)) && path[0] != '/')
	{
		joined_path = ft_strjoin(cwd, "/");
		if (cwd)
			free(cwd);
		joined_path = ft_strjoin(joined_path, path);
		if (!chdir(joined_path))
			return (free(joined_path), 0);
	}
	else
	{
		if (!chdir(path))
			return (free(cwd), 0);
	}
	return (1);
}

void	bi_exit(t_exec *exec)
{
	if (&exec - 1 || &exec + 1)
		return ;
	exit(1);
}
