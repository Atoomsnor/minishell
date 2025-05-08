/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:12:39 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/08 17:23:16 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

// open all outfiles with correct flags.
// int		open_outfiles(t_input *in)
// {
// 	t_input *cpy;
// 	int		fd;

// 	cpy = in;
// 	fd = 0;
// 	while (in)
// 	{
// 		if (in->type == t_append)
// 		{
// 			if (fd)
// 				close(fd);
// 			fd = open(in->next->txt, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		}
// 		else if (in->type == t_right)
// 		{
// 			if (fd)
// 				close(fd);
// 			fd = open(in->next->txt, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		}
// 		in = in->next;
// 	}
// 	return (fd);
// }

// void clear_outfiles(t_input *in)
// {
// 	t_input *cpy;

// 	cpy = in;
// 	while (cpy)
// 	{
// 		if (cpy->type == t_right || cpy->type == t_append)
// 		{
// 			if (cpy->next && cpy->next->next &&
// 				has_type(cpy->next->next, t_right, t_append))
// 				{
// 					cpy = cpy->next->next;
// 					ft_lstdelone(cpy->prev->prev);
// 					ft_lstdelone(cpy->prev);
// 					cpy = cpy->prev;
// 				}
				
// 		}
// 		cpy = cpy->next;
// 	}
// }

int	increment_to_nex(t_input *input)
{
	int i;
	t_input *in;

	in = input;
	i = 0;
	while (in && in->type != t_append && in->type != t_right && in->type != t_pipe)
	{
		in = in->next;
		i++;
	}
	return (i);
}

void run_command(t_input *input)
{
	(void)input;
}

void do_commands(t_shell *shell)
{
	t_input *input;
	t_fd	*fd;
	pid_t	pid;

	input = *shell->curr_input;
	fd = malloc(sizeof(t_fd));
	fd->in = open("./minishell", O_RDONLY);
	// fd->out = open_outfiles(input);
	// if (fd->out)
	// 	clear_outfiles(input);
	while (input)
	{
		if (pipe(fd->pipe) == -1)
			return ;
		pid = fork();
		if (pid == -1)
			return ;
		else if (input->type == t_left)
		{
			close(fd->in);
			fd->in = open(input->next->txt, O_RDONLY);
			input = input->next->next;
		}
		else if (input->type == t_pipe)
		{
			// next?
			// close(fd->pipe[0]);
			// run_command(input);
			// list_move(input, increment_to_nex(input));
		}
		else if (input->type == t_right)
		{
			fd->out = open(input->next->txt, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			input = input->next->next;
		}
		else if (input->type == t_append)
		{
			fd->out = open(input->next->txt, O_WRONLY | O_CREAT | O_APPEND, 0644);
			input = input->next->next;
		}
		if (input->type == t_txt)
		{
			// child?
			close(fd->pipe[0]);
			run_command(input);
			list_move(input, increment_to_nex(input));
		}
		
	}
}