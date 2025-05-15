/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:10:12 by roversch          #+#    #+#             */
/*   Updated: 2025/05/15 15:17:30 by roversch         ###   ########.fr       */
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

int find_in(t_input *input)
{
	int fd;

	fd = 0;
	while (input && input != input->head)
	{
		if (input->type == t_heredoc)
			fd = open("./minishell", O_RDONLY);
		else if (input->type == t_left)
			fd = open(input->next->txt, O_RDONLY);
		else if (input->type == t_pipe)
			fd = open(input->next->txt, O_RDONLY); // wrong for now, fix piping later
		if (fd)
			return (fd);
		input = input->prev;
	}
}

t_exec *fill_exec(t_input *input, int times)
{
	t_exec *cmd;
	int	count;
	int	i;
	while (times < 0)
	{
		if (input->type == t_pipe)
			times--;
		input = input->next;
	}
	cmd = ft_calloc(1, sizeof(t_exec));
	if (!cmd)
		return (NULL);
	count = count_till_pipe(input);
	cmd->full_cmd = ft_calloc(count, sizeof(char *));
	if (!cmd->full_cmd)
		return (NULL);
	cmd->infile = find_in();
	cmd->outfile = find_out();
	cmd->full_path = find_path();
	i = 0;
	while (i < count)
	{
		cmd->full_cmd[i] = ;
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
		cmd[i] = fill_exec(*input, i);
		i++;
	}
}