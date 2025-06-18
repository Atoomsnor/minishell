/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:29:53 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/18 16:50:07 by nhendrik         ###   ########.fr       */
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
	static int	hist_count = 0;
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

void	add_heredoc_hist(int fd, char **hist)
{
	char	*in;

	in = NULL;
	read(fd, in, INT_MAX);
	if (fd < 0)
		return ;
	printf("HISTORY: %s\n", in);
	history(in, hist);
	if (in)
		free(in);
}