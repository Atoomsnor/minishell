/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:16:34 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/23 17:29:37 by nhendrik         ###   ########.fr       */
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
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		write(STDOUT_FILENO, "\n", 1);
		//rl_done = 1;
	}
}

int	here_child(char *delimiter)
{
	char	*quote;
	int		pipefd[2];

	quote = NULL;
	if (pipe(pipefd) == -1)
		return (-1);
	while (!g_signalreceived)
	{
		quote = readline("> ");
		if (!quote)
		{
			if (g_signalreceived)
				g_signalreceived = 0;
			else 
				break ;
		}
		write(pipefd[1], "\n", 1);
		write(pipefd[1], quote, ft_strlen(quote));
		if (!ft_strncmp(quote, delimiter, ft_strlen(delimiter)))
			break ;
	}
	close(pipefd[1]);
	if (g_signalreceived)
		g_signalreceived = 0;
	return (pipefd[0]);
}

int	run_here_doc(t_input **input, char *delimiter, char **hist)
{
	signal(SIGINT, heredocsig);
	// printf("here\n");
	(*input)->hd_fd = here_child(delimiter);
	if ((*input)->hd_fd < 0)
		return (-1);
	// printf("outofheredoc %i\n", (*input)->hd_fd);
	add_heredoc_hist((*input)->hd_fd, hist);
	signal(SIGINT, sigint_handler);
	return ((*input)->hd_fd);
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