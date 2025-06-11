/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:29:53 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/11 19:28:04 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

void	history(char *in, char **hist)
{
	int	hist_count;
	int			i;

	i = 1;
	if (hist_count < HISTORY_SIZE)
	{
		hist[hist_count] = in;
		add_history(in);
		hist_count++;
	}
	else
	{
		rl_clear_history();
		while (i < HISTORY_SIZE)
		{
			hist[i - 1] = hist[i];
			add_history(hist[i - 1]);
			i++;
		}
		hist[HISTORY_SIZE - 1] = in;
		add_history(in);
	}
}

void	 save_history(char *in, int send, char **hist)
{
	// static char *hist = NULL;

	if (in)
		*hist = ft_strjoin(*hist, in);
	if (send == 1)
	{
		history(in, hist);
		free(hist);
		hist = NULL;
	}
	if (send == -1)
	{
		free(hist);
		hist = NULL;
	}
}

void	add_heredoc_hist(int fd, char **hist)
{
	char	*in;

	read(fd, in, INT_MAX);
	printf("%s\n", in);
	save_history(in, 0, hist);
}