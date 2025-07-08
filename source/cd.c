/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:50:13 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/08 16:19:07 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void	change_env_var(char ***env, char *var_name, char *content)
{
	int	i;
	int	len;

	i = 0;
	while ((*env)[i])
	{
		len = has_char((*env)[i], '=');
		if (!ft_strncmp((*env)[i], var_name, ft_strlen(var_name)))
			(*env)[i] = ft_strjoin_free(ft_substr_free((*env)[i], 0, len + 1),
					content, 1);
		i++;
	}
}

static int	home_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp("HOME=", env[i], 5))
		{
			if (chdir(ft_substr(env[i], 5, ft_strlen(env[i]) - 5)) == 0)
				return (1);
			else
				break ;
		}
		i++;
	}
	return (0);
}

static int	absolute_path(char *path, char ***env, char *cwd)
{
	char	*joined_path;

	joined_path = ft_strjoin(cwd, "/");
	if (cwd)
		free_and_null(cwd);
	joined_path = ft_strjoin_free(joined_path, path, 1);
	if (!joined_path)
		return (0);
	if (chdir(joined_path) == -1)
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free_and_null(joined_path);
		return (0);
	}
	change_env_var(env, "PWD", joined_path);
	free(joined_path);
	return (1);
}

int	cd(char **path, char ***env)
{
	char	*cwd;

	if (!path || !path[1] || (path[1][0] == '~' && path[1][1] == '\0')
			|| !ft_strncmp(path[1], "~/", 3))
		return (home_path(*env));
	if (path[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2), 0);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	change_env_var(env, "OLDPWD", cwd);
	if (ft_strncmp(cwd, path[1], ft_strlen(cwd)) && path[1][0] != '/')
		return (absolute_path(path[1], env, cwd));
	else
	{
		if (chdir(path[1]) == -1)
			return (ft_putstr_fd(path[1], 2),
				ft_putstr_fd(": No such file or directory\n", 2), free(cwd), 0);
		change_env_var(env, "PWD", path[1]);
	}
	free(cwd);
	return (1);
}
