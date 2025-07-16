/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:03:53 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/16 13:07:08 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <fcntl.h>

static int	find_out(char *path, int *fd, t_type type)
{
	if (*fd > 1)
		close(*fd);
	if (access(path, F_OK) == 0)
		if (access(path, W_OK) != 0)
			return (-2);
	if (type == t_right)
		*fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == t_append)
		*fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (*fd);
}

static int	find_in(char *path, int *fd)
{
	if (*fd > 1)
		close(*fd);
	if (access(path, F_OK) != 0)
		return (-1);
	if (access(path, R_OK) != 0)
		return (-2);
	*fd = open(path, O_RDONLY);
	return (*fd);
}

t_input	*rotate_backwards(t_input *input)
{
	while (input && input->type != t_pipe)
	{
		if (input->prev && input->prev->type != t_pipe)
			input = input->prev;
		else
			return (input);
	}
	return (input);
}

static int	hd_fd(int in_fd, int hd_fd)
{
	if (in_fd > 1)
		close(in_fd);
	return (hd_fd);
}

int	find_in_out(t_input *input, int *in_fd, int *out_fd, char **error_msg)
{
	*in_fd = 0;
	*out_fd = 1;
	input = rotate_backwards(input);
	if (!input)
		return (-1);
	if (input->type == t_pipe)
		input = input->next;
	while (input && input->type != t_pipe)
	{
		if ((input->type == t_right || input->type == t_append) && input->next)
			*out_fd = find_out(input->next->txt, out_fd, input->type);
		else if (input->type == t_left)
			*in_fd = find_in(input->next->txt, in_fd);
		else if (input->type == t_heredoc)
			*in_fd = hd_fd(*in_fd, input->hd_fd);
		if (*out_fd == -1 || *in_fd == -1)
			return (adjust_error(error_msg, input->next->txt,
					": No such file or directory\n"), -1);
		if (*out_fd < 0)
			return (*out_fd);
		if (*in_fd < 0)
			return (*in_fd);
		input = input->next;
	}
	return (0);
}
