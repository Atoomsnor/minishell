/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:16:34 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/09 17:13:23 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void	heredocsig(int signal)
{
	if (signal == SIGINT)
	{
		g_signalreceived = signal;
		// rl_replace_line("", 0);
		printf("\n");
		
	}
}

void *here_child(t_input **input, char *delimiter)
{
	char	*quote;

	quote = NULL;
	while (!g_signalreceived && ft_strncmp(quote, delimiter, ft_strlen(delimiter)))
	{
		quote = readline("> ");
		if (!quote)
		{
			if (g_signalreceived)
			{
				g_signalreceived = 0;
				exit(1);
			}
			else 
				break ;
		}
		(*input)->txt = ft_strjoin((*input)->txt, "\n");
		(*input)->txt = ft_strjoin((*input)->txt, quote);
	}
	if (g_signalreceived)
	{
		g_signalreceived = 0;
		exit(1);
	}
	
	exit(1);
	return (NULL);
}

void	*run_here_doc(t_input **input, char *delimiter)
{
	pid_t	pid;	

	signal(SIGINT, heredocsig);
	pid = fork();
	if (pid == -1)
		return (NULL);
	if (pid == 0)
		here_child(input, delimiter);
	signal(SIGINT, sigint_handler);
	return (NULL);
}

void *quote_child(t_input **input, char quote_type)
{
	char	*quote;

	quote = NULL;
	printf("quotechild\n");
	while (!g_signalreceived && has_char(quote, quote_type) < 0)
	{
		quote = readline("> ");
		if (!quote)
		{
			if (g_signalreceived)
			{
				g_signalreceived = 0;
				exit(1);
			}
			else 
				break ;
		}
		(*input)->txt = ft_strjoin((*input)->txt, "\n");
		(*input)->txt = ft_strjoin((*input)->txt, quote);
	}
	exit(1);
	return (NULL);
}

void	*run_quote_doc(t_input **input, char quote_type)
{
	pid_t	pid;	

	printf("haHAA quotetype %c\n", quote_type);
	signal(SIGINT, heredocsig);
	if (g_signalreceived)
		g_signalreceived = 0;
	pid = fork();
	if (pid == -1)
		return (NULL);
	if (pid == 0)
		quote_child(input, quote_type);
	wait(NULL);
	if (g_signalreceived)
		g_signalreceived = 0;
	signal(SIGINT, sigint_handler);
	return (NULL);
}