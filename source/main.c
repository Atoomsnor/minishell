/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/04/23 11:08:49 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

static int	g_signalreceived = 0;

// function called when signals are received
void	sighandler(int signal)
{
	g_signalreceived = signal;
}

// create signals that need to be handled
void	init_signals(void)
{
	signal(SIGUSR1, sighandler);
	signal(SIGUSR2, sighandler);
}

void	redo_hist(char **hist, char *in)
{
	int i;

	i = 1;
	rl_clear_history();
	while (hist[i])
	{
		add_history(hist[i]);
		i++;
	}
	i = 0;
	while (i < 29)
	{
		hist[i] = hist[i + 1];
		i++;
	}
	printf("i: %i\n", i);
	if (i == 29)
	{
		hist[i] = in;
		add_history(in);
	}
}

void	history(char *in)
{
	static int hist_count = 0;
	static char *hist[31];

	hist[30] = "\0";
	if (hist_count < 30)
	{
		add_history(in);
		hist[hist_count] = in;
		hist_count++;
	}
	printf("hc: %i\n", hist_count);
	while (hist_count > 29)
	{
		redo_hist(hist, in);
		hist_count--;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*in;
	(void)argc;
	(void)argv;
	in = NULL;
	init_signals();
	while (1)
	{
		in = readline("megashell>$ ");
		if (in && in[0] != '\0')
		{
			history(in);
			if (in[0] == '<')
				pipe_parser(in, envp);
			else
				singlecmd(in, envp);
			if (g_signalreceived == SIGUSR1)
				g_signalreceived = 0;
		}
	}
}
