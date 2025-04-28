/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/04/28 14:13:23 by nhendrik         ###   ########.fr       */
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

void	history(char *in)
{
	static int	hist_count = 0;
	static char	*hist[HISTORY_SIZE];
	int	i;

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

int	main(int argc, char **argv, char **envp)
{
	char	*in;
	t_input	*parsed;

	(void)argc;
	(void)argv;
	in = NULL;
	parsed = NULL;
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
			{
				parsed = init_list(in);
				// while (parsed)
				// {
				// 	printf("%s\n", *parsed);
				// 	singlecmd(*parsed, envp);
				// 	parsed++;
				// }
			}
			if (g_signalreceived == SIGUSR1)
				g_signalreceived = 0;
		}
	}
}
