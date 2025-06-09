/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:33:15 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/09 15:45:39 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <signal.h>
#include <readline/readline.h>
#include <stdio.h>

volatile sig_atomic_t	g_signalreceived = 0;

// function called when signals are received
void	sighandler(int signal)
{
	if (signal == SIGINT)
	{
		g_signalreceived = 1;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sigint_handler(int signal)
{
	(void)signal;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// create signals that need to be handled
void	init_signals(void)
{
	// struct sigaction	sa;

	// sa.sa_handler = sigint_handler;
	// sigemptyset(&sa.sa_mask);
	// sa.sa_flags = SA_RESTART;
	// sigaction(SIGINT, &sa, NULL);
	rl_catch_signals  = 1;
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);
	signal(SIGUSR2, SIG_IGN);
}
