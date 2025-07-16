/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:50:13 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/16 18:06:23 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void	change_env_var(char ***env, char *var_name, char *content)
{
	int	i;
	int	len;

	i = 0;
	if (!content)
		return ;
	while ((*env)[i])
	{
		len = has_char((*env)[i], '=');
		if (!ft_strncmp((*env)[i], var_name, ft_strlen(var_name)))
		{
			(*env)[i] = ft_strjoin_free(ft_substr_free((*env)[i], 0, len + 1),
					content, 1);
			if (!(*env)[i])
			{
				free_array(*env);
				return ;
			}
		}
		i++;
	}
}

static int	home_path(char **env)
{
	char	*substr;
	int		i;

	i = 0;
	substr = NULL;
	while (env[i])
	{
		if (!ft_strncmp("HOME=", env[i], 5))
		{
			substr = ft_substr(env[i], 5, ft_strlen(env[i]) - 5);
			if (!substr)
				return (0);
			if (chdir(substr) == 0)
				return (free_and_null(substr), 1);
			else
				break ;
			free_and_null(substr);
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
	if (!joined_path)
		return (0);
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
	free_and_null(joined_path);
	joined_path = getcwd(NULL, 0);
	if (!joined_path)
		return (0);
	change_env_var(env, "PWD", joined_path);
	free_and_null(joined_path);
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
	if (!*env)
		return (free(cwd), 0);
	if (ft_strncmp(cwd, path[1], ft_strlen(cwd)) && path[1][0] != '/')
		return (absolute_path(path[1], env, cwd));
	if (chdir(path[1]) == -1)
		return (ft_putstr_fd(path[1], 2),
			ft_putstr_fd(": No such file or directory\n", 2), free(cwd), 0);
	change_env_var(env, "PWD", path[1]);
	if (!*env)
		return (free(cwd), 0);
	free(cwd);
	return (1);
}
