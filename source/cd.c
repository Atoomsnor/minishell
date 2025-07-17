/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:50:13 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/17 12:21:30 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	mat_len(char **mat)
{
	int	i;

	if (!mat)
		return (0);
	i = 0;
	while (mat[i])
		i++;
	return (i);
}

static void	change_env_var(char ***env, char *var_name, char *content)
{
	const int	envlen = mat_len(*env);
	int			len;
	int			i;

	i = 0;
	if (!content || !env || !*env)
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
				free_env(env, envlen);
				return ;
			}
		}
		i++;
	}
}

static int	home_path(char ***env, char *cwd)
{
	char	*substr;
	int		i;

	i = -1;
	substr = NULL;
	if (!cwd)
		return (malloc_error_free(NULL), 0);
	while ((*env)[++i])
	{
		if (!ft_strncmp("HOME=", (*env)[i], 5))
		{
			substr = ft_substr((*env)[i], 5, ft_strlen((*env)[i]) - 5);
			if (!substr)
				return (malloc_error_free(free_and_null(cwd)), 0);
			if (chdir(substr) == 0)
				return (change_env_var(env, "OLDPWD", cwd),
					change_env_var(env, "PWD", substr),
					free_and_null(cwd), free_and_null(substr), 1);
			else
				break ;
			free_and_null(substr);
		}
	}
	free_and_null(cwd);
	return (0);
}

static int	relative_path(char *path, char ***env, char *cwd)
{
	char	*joined_path;

	joined_path = ft_strjoin(cwd, "/");
	if (cwd)
		free_and_null(cwd);
	if (!joined_path)
		return (malloc_error_free(NULL), 0);
	joined_path = ft_strjoin_free(joined_path, path, 1);
	if (!joined_path)
		return (malloc_error_free(NULL), 0);
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
		return (malloc_error_free(NULL), 0);
	change_env_var(env, "PWD", joined_path);
	free_and_null(joined_path);
	return (1);
}

int	cd(char **path, char ***env)
{
	char	*cwd;

	if (!path || !path[1] || (path[1][0] == '~' && path[1][1] == '\0')
			|| !ft_strncmp(path[1], "~/", 3))
		return (home_path(env, getcwd(NULL, 0)));
	if (path[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2), 0);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (malloc_error_free(NULL), 0);
	change_env_var(env, "OLDPWD", cwd);
	if (!*env)
		return (malloc_error_free(free_and_null(cwd)), 0);
	if (ft_strncmp(cwd, path[1], ft_strlen(cwd)) && path[1][0] != '/')
		return (relative_path(path[1], env, cwd));
	if (chdir(path[1]) == -1)
		return (ft_putstr_fd(path[1], 2),
			ft_putstr_fd(": No such file or directory\n", 2), free(cwd), 0);
	change_env_var(env, "PWD", path[1]);
	if (!*env)
		return (malloc_error_free(free_and_null(cwd)), 0);
	free(cwd);
	return (1);
}
