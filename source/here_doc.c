/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:16:34 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/11 19:19:31 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	heredocsig(int signal)
{
	if (signal == SIGINT)
	{
		g_signalreceived = signal;
		// rl_replace_line("", 0);
		printf("\n");
		exit(1);
	}
}

void *here_child(char *delimiter, int fd)
{
	char	*quote;

	quote = NULL;
	while (!g_signalreceived)
	{
		quote = readline("> ");
		if (!quote)
		{
			if (g_signalreceived)
			{
				g_signalreceived = 0;
				close(fd);
				exit(1);
			}
			else 
				break ;
		}
		if (!ft_strncmp(quote, delimiter, ft_strlen(delimiter)))
			break ;
		ft_putendl_fd(quote, fd);
	}
	if (g_signalreceived)
	{
		g_signalreceived = 0;
		close(fd);
		exit(1);
	}
	close(fd);
	exit(1);
	return (NULL);
}

int		run_here_doc(char *delimiter, char **hist)
{
	pid_t	pid;
	int		pipefd[2];

	printf("DELIMITER: %s\n", delimiter);
	if (g_signalreceived)
		g_signalreceived = 0;
	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
	{
		close(pipefd[0]);
		signal(SIGINT, heredocsig);
		here_child(delimiter, pipefd[1]);
	}
	else 
		signal(SIGINT, SIG_IGN);
	wait(NULL);
	if (g_signalreceived)
		g_signalreceived = 0;
	else
		add_heredoc_hist(pipefd[0], hist);
	signal(SIGINT, sigint_handler);
	close(pipefd[1]);
	return (pipefd[0]);
}

// void *quote_child(t_input **input, char quote_type)
// {
// 	char	*quote;

// 	quote = NULL;
// 	printf("quotechild\n");
// 	while (!g_signalreceived && has_char(quote, quote_type) < 0)
// 	{
// 		quote = readline("> ");
// 		if (!quote)
// 		{
// 			if (g_signalreceived)
// 			{
// 				g_signalreceived = 0;
// 				exit(1);
// 			}
// 			else 
// 				break ;
// 		}
// 		(*input)->txt = ft_strjoin((*input)->txt, "\n");
// 		(*input)->txt = ft_strjoin((*input)->txt, quote);
// 	}
// 	exit(1);
// 	return (NULL);
// }

// void	*run_quote_doc(t_input **input, char quote_type)
// {
// 	pid_t	pid;	

// 	printf("haHAA quotetype %c\n", quote_type);
// 	signal(SIGINT, heredocsig);
// 	if (g_signalreceived)
// 		g_signalreceived = 0;
// 	pid = fork();
// 	if (pid == -1)
// 		return (NULL);
// 	if (pid == 0)
// 		quote_child(input, quote_type);
// 	wait(NULL);
// 	if (g_signalreceived)
// 		g_signalreceived = 0;
// 	signal(SIGINT, sigint_handler);
// 	return (NULL);
// }