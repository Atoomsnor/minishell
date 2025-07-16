/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   murder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:28:35 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/16 12:58:19 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

void	close_exec_fds(t_exec *exec)
{
	if (!exec)
		return ;
	if (exec->in_fd != 0)
	{
		close(exec->in_fd);
		exec->in_fd = 0;
	}
	if (exec->out_fd != 1)
	{
		close(exec->out_fd);
		exec->out_fd = 1;
	}
}

void	lynch_exec(t_exec **exec)
{
	t_exec	**head;
	int		i;

	if (!exec)
		return ;
	head = exec;
	while (*exec)
	{
		close_exec_fds((*exec));
		if ((*exec)->err_msg)
			free_and_null((*exec)->err_msg);
		if ((*exec)->full_path && (*exec)->full_path[0] != '\0')
			free_and_null((*exec)->full_path);
		if ((*exec)->full_cmd)
		{
			i = 0;
			while ((*exec)->full_cmd[i])
				free_and_null((*exec)->full_cmd[i++]);
			free_and_null((*exec)->full_cmd);
		}
		if ((*exec))
			free_and_null((*exec));
		exec++;
	}
	free_and_null(head);
}

void	*shank_input(t_input **input)
{
	t_input	*next;

	if (!input)
		return (NULL);
	next = NULL;
	if (*input && (*input)->head && *input != (*input)->head)
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
	return (NULL);
}

void	*burn_history(t_history *hist)
{
	int	i;

	i = 0;
	if (hist)
	{
		if (hist->in)
			free_and_null(hist->in);
		while (hist->hist[i])
		{
			free_and_null(hist->hist[i]);
			i++;
		}
		free_and_null(hist);
	}
	rl_clear_history();
	return (NULL);
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
