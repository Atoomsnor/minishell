/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   murder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:28:35 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/03 18:48:20 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>

void	shoot_error(char *error)
{
	(void)error;
}

// void	close_exec_fds(t_exec *exec)
// {
// 	if (!exec)
// 		return ;
// 	if (exec->in_fd != -1)
// 	{
// 		close(exec->in_fd);
// 		exec->in_fd = -1;
// 	}
// 	if (exec->out_fd != -1)
// 	{
// 		close(exec->out_fd);
// 		exec->out_fd = -1;
// 	}
// }

void	lynch_exec(t_exec **exec)
{
	int	i;
	int	j;

	i = 0;
	if (!exec)
		return ;
	while (exec[i])
	{
		// close_exec_fds(exec[i]);
		if (exec[i]->full_path && exec[i]->full_path[0] != '\0')
			free_and_null(exec[i]->full_path);
		if (exec[i]->full_cmd)
		{
			j = 0;
			while (exec[i]->full_cmd[j])
			{
				free_and_null(exec[i]->full_cmd[j]);
				j++;
			}
			free_and_null(exec[i]->full_cmd);
		}
		// close_exec_fds(exec[i]);
		if (exec[i])
			free_and_null(exec[i]);
		exec[i] = NULL;
		i++;
	}
	// close_exec_fds(exec[i]);
	free_and_null(exec[i]);
	free_and_null(exec);
}

void	shank_input(t_input **input)
{
	t_input	*next;

	if (!input)
		return ;
	next = NULL;
	if (*input && (*input)->head &&  *input != (*input)->head)
		*input = (*input)->head;
	while (*input)
	{
		if ((*input)->txt)
			free_and_null((*input)->txt);
		next = (*input)->next;
		free_and_null((*input));
		(*input) = next;
	}
	free_and_null(input);
}

void	burn_history(t_history *hist)
{
	int	i;

	i = 0;
	if (hist)
	{
		if (hist->in)
		{
			free_and_null(hist->in);
		}
		while (hist->hist[i])
		{
			free_and_null(hist->hist[i]);
			i++;
		}
		free_and_null(hist);
	}
	rl_clear_history();
}

void	*die(t_exec **exec, t_input **input, char *error, void *ret)
{
	(void)ret;
	if (error != NULL)
	{
		ft_putstr_fd(error, 2);
		free_and_null(error);
	}
	if (exec)
		lynch_exec(exec);
	if (input)
		shank_input(input);
	return (NULL);
}
