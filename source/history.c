/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:29:53 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/08 16:22:05 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

void	history(t_history *hist)
{
	int	i;

	i = 1;
	if (hist->hist_count < HISTORY_SIZE)
	{
		hist->hist[hist->hist_count] = hist->in;
		add_history(hist->in);
		hist->hist_count++;
	}
	else
	{
		rl_clear_history();
		while (i < HISTORY_SIZE)
		{
			hist->hist[i - 1] = hist->hist[i];
			add_history(hist->hist[i - 1]);
			i++;
		}
		hist->hist[HISTORY_SIZE - 1] = hist->in;
		add_history(hist->in);
	}
}

t_history	*init_hist(void)
{
	t_history	*hist;

	hist = ft_calloc(1, sizeof(t_history));
	hist->hist_count = 0;
	hist->in = NULL;
	return (hist);
}
