/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:11:18 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/17 11:18:49 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	rotate_input(t_input **input)
{
	if ((*input)->type == t_pipe)
		*input = (*input)->next;
	while (*input)
	{
		if ((*input) && (*input)->next && has_redirs(*input)
			&& (!(*input)->next->next || (*input)->next->next->type == t_pipe))
		{
			if ((*input)->prev)
				(*input) = (*input)->prev;
			return ;
		}
		else if (((*input)->type == t_flag || (*input)->type == t_txt)
			&& (*input)->prev && ((*input)->prev->type == t_left
				|| (*input)->prev->type == t_heredoc))
			(*input) = (*input)->next;
		else if (((*input)->type == t_flag || (*input)->type == t_txt)
			&& (*input)->prev && ((*input)->prev->type == t_right
				|| (*input)->prev->type == t_append))
			(*input) = (*input)->next;
		else if ((*input)->type == t_flag || (*input)->type == t_txt)
			return ;
		else
			(*input) = (*input)->next;
	}
}

int	rotate_past_pipe(t_input **input, int count)
{
	while (*input && (*input)->type != t_pipe)
		*input = (*input)->next;
	if (*input && (*input)->type == t_pipe)
		*input = (*input)->next;
	return (count - 1);
}

int	has_type(t_input *input, t_type type)
{
	t_input	*cpy;

	cpy = input;
	while (cpy)
	{
		if (cpy->type == type)
			return (1);
		cpy = cpy->next;
	}
	return (0);
}

int	count_cmds(t_input *input)
{
	int	count;

	count = 1;
	while (input)
	{
		if (input->type == t_pipe && input->next)
			count++;
		input = input->next;
	}
	return (count);
}

int	count_till_pipe(t_input *input)
{
	t_input	*cpy;
	int		count;

	count = 0;
	cpy = input;
	cpy = rotate_backwards(cpy);
	while (cpy && (cpy->type != t_pipe))
	{
		if (has_redirs(cpy))
		{
			if (cpy->next && cpy->next->next)
				cpy = cpy->next->next;
			else
				return (count);
		}
		else if (cpy->type == t_txt || cpy->type == t_flag)
		{
			count++;
			cpy = cpy->next;
		}
		else
			return (count);
	}
	return (count);
}
