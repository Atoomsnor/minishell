/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   murder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:28:35 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/17 13:45:51 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void	shoot_error(va_list *ptr)
{
	char *error;

	while (1)
	{
		error = va_arg(*ptr, char *);
		if (error == NULL)
			break ;
		ft_putstr_fd(error, 2);
	}
	ft_putstr_fd("\n", 2);
	va_end(*ptr);
}

void	lynch_exec(t_exec **exec)
{
	int	i;
	int	j;

	i = 0;
	if (!exec)
		return ;
	while (exec[i])
	{
		if (exec[i]->full_path && exec[i]->full_path[0] != '\0')
			free(exec[i]->full_path);
		if (exec[i]->full_cmd)
		{
			j = 0;
			while (exec[i]->full_cmd[j])
			{
				free(exec[i]->full_cmd[j]);
				j++;
			}
			free(exec[i]->full_cmd);
		}
		free(exec[i]);
		exec[i] = NULL;
		i++;
	}
	free(exec[i]);
	free(exec);
}

void	shank_input(t_input **input)
{
	t_input	*next;

	if (!input)
		return ;
	next = NULL;
	if (*input && *input != (*input)->head)
		*input = (*input)->head;
	while (*input)
	{
		if ((*input)->txt)
			free((*input)->txt);
		next = (*input)->next;
		free((*input));
		(*input) = next;
	}
	free(input);
}

void	die(t_exec **exec, t_input **input, ...)
{
	va_list ptr;

	va_start(ptr, input);
	shoot_error(&ptr);
	if (exec)
		lynch_exec(exec);
	if (input)
		shank_input(input);
}
