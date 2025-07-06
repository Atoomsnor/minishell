/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:15:06 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/03 18:07:52 by nhendrik         ###   ########.fr       */
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

static void change_env_var(char ***env, char *var_name, char *content)
{
	int	i;
	int	len;

	i = 0;
	while ((*env)[i])
	{
		len = has_char((*env)[i], '=');
		if (!ft_strncmp((*env)[i], var_name, ft_strlen(var_name)))
			(*env)[i] = ft_strjoin_free(ft_substr_free((*env)[i], 0, len + 1), content, 1);
		i++;
	}
}

static int its_coming_home(char **env)
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

int	cd(char **path, char ***env)
{
	char	*cwd;
	char	*joined_path;

	if (!path || !path[1] || (path[1][0] == '~' && path[1][1] == '\0') || !ft_strncmp(path[1], "~/", 3))
		return (its_coming_home(*env));
	if (path[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2), 0);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	joined_path = NULL;
	change_env_var(env, "OLDPWD", cwd);
	if (ft_strncmp(cwd, path[1], ft_strlen(cwd)) && path[1][0] != '/')
	{
		joined_path = ft_strjoin(cwd, "/");
		if (cwd)
			free(cwd);
		joined_path = ft_strjoin_free(joined_path, path[1], 1);
		if (chdir(joined_path) == -1)
			return (ft_putstr_fd(path[1], 2), ft_putstr_fd(": No such file or directory\n", 2), free(joined_path), 0);
		change_env_var(env, "PWD", joined_path);
	}
	else
	{
		if (chdir(path[1]) == -1)
			return (ft_putstr_fd(path[1], 2), ft_putstr_fd(": No such file or directory\n", 2), free(cwd), 0);
		change_env_var(env, "PWD", path[1]);
	}
	if (joined_path)
		free(joined_path);
	free(cwd);
	return (1);
}

void	bi_exit(t_exec *exec, int child)
{
	int	ret;
	int	i;

	if (child)
		return ;
	ret = 1;
	printf("exit\n");
	if (exec->full_cmd[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit (1);
	}
	if (exec->full_cmd[1])
	{
		i = 0;
		while (exec->full_cmd[1][i])
		{
			if (!ft_isdigit(exec->full_cmd[1][i]) && exec->full_cmd[1][i] != '+' && exec->full_cmd[1][i] != '-')
			{
				ft_putstr_fd("exit: ", 2);
				ft_putstr_fd(exec->full_cmd[1], 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				exit(2);
			}
			i++;
		}
		if (i > 0 && !exec->full_cmd[1][i])
			ret = ft_atoi(exec->full_cmd[1]);
	}
	exit(ret);
}
