/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:23:46 by roversch          #+#    #+#             */
/*   Updated: 2025/06/05 17:53:11 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char	**exporting(char *str, char **env)
{
	char	**new_env;
	int		len;
	int		i;

	if (str)
	{
		len = 0;
		while (env[len])
			len++;
		new_env = ft_calloc(sizeof(char *), len + 2);
		i = 0;
		while (env[i] && i < len - 1)
		{
			new_env[i] = env[i];
			i++;
		}
		new_env[i++] = ft_strdup(str);
		while (env[i - 1])
		{
			new_env[i] = env[i - 1];
			i++;
		}
		return (new_env);
	}
	return (NULL);
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

void	env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}
