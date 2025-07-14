/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 10:26:53 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/14 14:40:34 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <stdio.h>

static int	export_validity(char *str)
{
	int	has_alpha;
	int	len;
	int	i;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	if (len == 0)
		return (0);
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
	return (1);
}

static char	**make_environment(char ***env, char *str, int len)
{
	char	**new_env;
	int		i;
	int		j;

	new_env = ft_calloc(sizeof(char *), len + 2);
	if (!new_env)
		return (malloc_error_free(NULL), *env);
	i = 0;
	j = 0;
	while ((*env)[j])
	{
		if (i == len - 1)
		{
			new_env[i] = ft_strdup(str);
			if (!new_env[i++])
				return (malloc_error_free(free_array(new_env)), *env);
		}
		else
			new_env[i++] = (*env)[j++];
	}
	free(*env);
	return (new_env);
}

static int	exporting(char *str, char ***env)
{
	int		len;
	int		i;

	i = export_validity(str);
	if (!i)
		return (0);
	else if (i == -1)
		return (1);
	len = -1;
	i = has_char(str, '=');
	while ((*env)[++len])
	{
		if (!ft_strncmp((*env)[len], str, i) && (*env)[len][i] == '=')
		{
			free((*env)[len]);
			(*env)[len] = ft_strdup(str);
			if (!(*env)[len])
				return (0);
			return (1);
		}
	}
	*env = make_environment(env, str, len);
	if (!*env)
		return (0);
	return (1);
}

int	exports(char **str, char ***env)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!exporting(str[i], env))
			return (0);
		i++;
	}
	return (1);
}
