/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:23:46 by roversch          #+#    #+#             */
/*   Updated: 2025/07/01 18:22:00 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static int	export_validity(char *str)
{
	int	len;
	int	i;
	int	has_alpha;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	i = 0;
	has_alpha = 0;
	while (i < len)
	{
		if (!ft_isalpha(str[i]))
		{
			if (!has_alpha || !(ft_isdigit(str[i]) || str[i] == '_'))
				return (0);
		}
		else
			has_alpha = 1;
		i++;
	}
	if (!str[len])
		return (-1);
	if (str[len] == '=' && !str[len + 1])
		return (0);
	return (1);
}

static char	**make_environment(char ***env, char *str, int len)
{
	char	**new_env;
	int		i;
	int		j;

	new_env = ft_calloc(sizeof(char *), len + 2);
	i = 0;
	j = 0;
	while ((*env)[j])
	{
		if (i == len - 1)
			new_env[i++] = ft_strdup(str);
		else
			new_env[i++] = (*env)[j++];
	}
	return (new_env);
}

int	exporting(char *str, char ***env)
{
	int		len;
	int		i;

	if (!str)
		return (0);
	i = export_validity(str);
	if (!i)
		return (0);
	else if (i == -1)
		return (1);
	len = 0;
	i = has_char(str, '=');
	while ((*env)[len])
	{
		if (!ft_strncmp((*env)[len], str, i) && (*env)[len][i] == '=')
		{
			free((*env)[len]);
			(*env)[len] = ft_strdup(str);
			return (1);
		}
		len++;
	}
	*env = make_environment(env, str, len);
	return (1);
}

// works in sync with export, destroys a saved var
void	unset(char *name, char **env)
{
	int	len;
	int	i;

	if (name)
	{
		i = 0;
		len = 0;
		while (env[len])
			len++;
		while (env[i])
		{
			if (!ft_strncmp(name, env[i], ft_strlen(name)))
			{
				ft_memmove(&env[i], &env[i + 1], len - i);
				env[len - 1] = NULL;
				break ;
			}
			i++;
		}
	}
}

void	env(char **envp, int fd)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		ft_putendl_fd(envp[i], fd);
		i++;
	}
}
