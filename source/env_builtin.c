/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:23:46 by roversch          #+#    #+#             */
/*   Updated: 2025/07/14 10:26:44 by nhendrik         ###   ########.fr       */
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
