/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:29:53 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/11 15:28:05 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void	history(char *in)
{
	static char	*hist[HISTORY_SIZE];
	static int	hist_count;
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

void	 save_history(char *in, int send)
{
	static char *hist = NULL;

	if (in)
		hist = ft_strjoin(hist, in);
	printf("history! %s\n", hist);
	if (send == 1)
	{
		history(hist);
		free(hist);
		hist = NULL;
	}
	if (send == -1)
	{
		free(hist);
		hist = NULL;
	}
}

void	add_heredoc_hist(int fd)
{
	char	*in;

	printf("a\n");
	in = get_next_line(fd);
	printf("e %s\n", in);
	while (in)
	{
		printf("b\n");
		save_history(ft_strjoin("\n", in), 0);
		free(in);
		printf("c\n");
		in = get_next_line(fd);
		printf("d %s\n", in);
	}
}