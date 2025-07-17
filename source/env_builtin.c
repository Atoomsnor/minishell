/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:23:46 by roversch          #+#    #+#             */
/*   Updated: 2025/07/17 12:21:24 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <stdio.h>

void	unset(char *name, char ***env)
{
	int	len;
	int	i;

	if (name)
	{
		i = 0;
		len = 0;
		while ((*env)[len])
			len++;
		while ((*env)[i] && ft_strncmp(name, (*env)[i], ft_strlen(name)))
			i++;
		if (i == len)
			return ;
		free((*env)[i]);
		while (i < len && (*env)[i + 1])
		{
			(*env)[i] = (*env)[i + 1];
			i++;
		}
		(*env)[len - 1] = NULL;
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

void	free_env(char ***env, int len)
{
	int	i;

	if (!env || !*env)
		return ;
	i = 0;
	while (i < len)
	{
		if ((*env)[i])
			free_and_null((*env)[i]);
		i++;
	}
	free_and_null((*env));
	(*env) = NULL;
}
