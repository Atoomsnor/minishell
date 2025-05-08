/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:15:06 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/08 18:09:37 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <limits.h>
#include <unistd.h>

int pwd(int fd)
{
	char cwd[INT_MAX];

	if(!getcwd(cwd, INT_MAX))
		return (0);
	else
		ft_putendl_fd(cwd, fd);
	return (1);
}

int echo(int fd, char *flag, char *to_write)
{
	if (flag)
	{
		if (!ft_strncmp(flag, "-n", 3))
			ft_putstr_fd(to_write, fd);
		else
		 return (0);
	}
	else
		ft_putendl_fd(to_write, fd);
	return (1);
}

void unset(char *name)
{
	unlink(name);
}

int cd(char *path)
{
	char	cwd[INT_MAX];
	char	*joined_path;

	if(!getcwd(cwd, INT_MAX))
		return (0);
	if (ft_strncmp(cwd, path, ft_strlen(cwd)))
	{
		joined_path = ft_strjoin(cwd, path);
		if (!chdir(path))
			return (0);
	}
	else
	{
		if (!chdir(path))
			return (0);
	}
	return (1);
}