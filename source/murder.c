/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   murder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:28:35 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/30 16:56:33 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void	shoot_error(char *error)
{
	(void)error;
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
		// printf("e\n");
		if (exec[i]->full_path && exec[i]->full_path[0] != '\0')
			free(exec[i]->full_path);
		// printf ("f\n");
		if (exec[i]->full_cmd)
		{
			j = 0;
			// printf("how\n");
			while (exec[i]->full_cmd[j])
			{
				// printf ("is this segging %s\n", exec[i]->full_cmd[j]);
				free(exec[i]->full_cmd[j]);
				j++;
			}
			// printf ("I dont understand\n");
			free(exec[i]->full_cmd);
		}
		// printf("death c\n");
		if (exec[i])
			free(exec[i]);
		exec[i] = NULL;
		i++;
	}
	// printf("death d\n");
	//free(exec[i]);
	free(exec);
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
			free((*input)->txt);
		next = (*input)->next;
		free((*input));
		*input = NULL;
		(*input) = next;
	}
	free(input);
	input = NULL;
}

void	*die(t_exec **exec, t_input **input, char *error, void *ret)
{
	(void)ret;
	if (error != NULL)
		ft_putstr_fd(error, 2);
	// printf("death a\n");
	if (exec)
		lynch_exec(exec);
	// printf("death n\n");
	if (input)
		shank_input(input);
	return (NULL);
}
