/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/04/14 21:35:54 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

static int g_signalreceived = 0;

void sighandler(int signal)
{
	g_signalreceived = signal;
}

void init_signals(void)
{
	signal(SIGUSR1, sighandler);
	signal(SIGUSR2, sighandler);
}

int main (int argc, char **argv, char **envp)
{
	char *in;

	(void)argc;
	(void)argv;
	in = NULL;
	init_signals();
	while (1)
	{
		in = readline("megashell>$ ");
		if (in[0] == '<')
			pipe_parser(in, envp);
	}
}