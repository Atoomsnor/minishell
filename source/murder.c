/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   murder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:28:35 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/23 15:22:49 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void	shoot_error(t_error error)
{
	if (error == error_input_fail)
		perror("Error input fail\n");
	else if (error == error_cmd_to_path)
		perror("Error cmd to path\n");
	else if (error == error_fill_exec)
		perror("Error fill_exec\n");
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
		}
		free(exec[i]);
		exec[i] = NULL;
		i++;
	}
	if (exec)
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

void	die(t_exec **exec, t_input **input, t_error error)
{
	if (exec)
		lynch_exec(exec);
	if (input)
		shank_input(input);
	shoot_error(error);
}
