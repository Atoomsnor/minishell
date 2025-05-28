/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   murder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:28:35 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/28 17:41:46 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int	blast_error(t_error error, t_exec *exec)
{
	if (!exec || !exec->full_cmd || !exec->full_cmd[0])
		return (0); 
	if (error == error_input_fail)
		ft_putstr_fd("Error input fail", 2);
	else if (error == error_cmd_to_path)
		print_strings_fd(2, exec->full_cmd[0], ": ", "command not found\n", NULL);
	else if (error == error_fill_exec)
		perror("Error fill_exec");
	return (1);
}

void	shoot_error(t_error error)
{
	if (error == error_input_fail)
		ft_putstr_fd("Error input fail", 2);
	else if (error == error_cmd_to_path)
		perror("Command not found");
	else if (error == error_fill_exec)
		perror("Error fill_exec");
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
			free(exec[i]->full_cmd[j]);
		}
		free(exec[i]);
		exec[i] = NULL;
		i++;
	}
	free(exec[i]);
	if (exec)
		free(exec);
}

void	shank_input(t_input **input)
{
	t_input	*next;

	printf("ye\n");
	if (!input)
		return ;
	next = NULL;
	if (*input && *input != (*input)->head)
		*input = (*input)->head;
	printf("ye2\n");
	while (*input)
	{
		printf("ye3\n");
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
	{
		if (!blast_error(error, *exec))
			shoot_error(error);
		lynch_exec(exec);
	}
	else
		shoot_error(error);
	if (input)
		shank_input(input);
}
