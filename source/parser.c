/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:10:12 by roversch          #+#    #+#             */
/*   Updated: 2025/05/15 15:44:07 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>

int count_cmds(t_input *input)
{
	int count;

	count = 1;
	while (input)
	{
		if (input->type == t_pipe)
			count++;
		input = input->next;
	}
	return (count);
}

int count_till_pipe(t_input *input)
{
	t_input *cpy;
	int count;

	count = 0;
	cpy = input;
	while (cpy && (cpy->type == t_txt || cpy->type == t_flag))
	{
		count++;
		cpy = cpy->next;
	}
	return (count);
}

t_exec *fill_exec(t_input *input, int times)
{
	t_exec *cmd;
	int	count;
	int	i;

	i = 0;
	while (times > 0)
	{
		if (input->type == t_pipe)
			times--;
		input = input->next;
	}
	count = count_till_pipe(input);
	cmd = ft_calloc(1, sizeof(t_exec));
	if (!cmd)
		return (NULL);
	cmd->full_cmd = ft_calloc(count, sizeof(char *));
	if (!cmd->full_cmd)
		return (NULL);
	while (input && input->type != t_pipe)
	{
		if (input->type == t_txt || input->type == t_flag)
			cmd->full_cmd[i++] = ft_strdup(input->txt);
		if (input->type == t_left)
			cmd->infile_path = ft_strdup(input->next->txt);
		if (input->type == t_right)
			cmd->outfile_path = ft_strdup(input->next->txt);
		if (input->type == t_append)
		{
			cmd->outfile_path = ft_strdup(input->next->txt);
			cmd->append_mode = 1;
		}
		if (input->type == t_heredoc)
		{
			cmd->infile_path = ft_strdup(".heredoc_tmp");
			cmd->heredoc = 1;
		}
		input = input->next;
	}
}

t_exec **tokens_to_exec(t_input **input)
{
	t_exec	**cmds;
	int	count;
	int	i;

	count = count_cmds(*input);
	cmds = ft_calloc(count + 1, sizeof(t_exec *));
	i = 0;
	while (i < count)
	{
		cmds[i] = fill_exec(*input, i);
		i++;
	}
}