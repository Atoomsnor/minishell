/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:16:34 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/02 13:07:44 by nhendrik         ###   ########.fr       */
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

static void	heredocsig(int signal)
{
	if (signal == SIGINT)
	{
		g_signalreceived = signal;
		// rl_done = 1;
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		write(STDOUT_FILENO, "\n", 1);
	}
}

static int	here_child(char *delimiter, int retval, char quotetype, char **env)//TODO namechange
{
	char	*input;
	int		pipefd[2];

	input = NULL;
	if (pipe(pipefd) == -1)
		return (-1);
	while (!g_signalreceived)
	{
		input = readline("> ");
		if (!input)
		{
			if (g_signalreceived)
				g_signalreceived = 0;
			else 
				break ;
		}
		if (!ft_strncmp(input, delimiter, ft_strlen(delimiter)))
		{
			free(input);
			break ;
		}
		if (has_char(input, '$') >= 0 && quotetype != '\'')
			input = handle_wildcard(input, env, retval);
		write(pipefd[1], input, ft_strlen(input));
		write(pipefd[1], "\n", 1);
		free(input);
	}
	close(pipefd[1]);
	if (g_signalreceived)
		g_signalreceived = 0;
	return (pipefd[0]);
}

int	run_here_doc(t_input **input, t_history *hist, int retval, char **env)
{
	char	quotetype;
	char	*delimiter;

	(void)hist;
	signal(SIGINT, heredocsig);
	delimiter = (*input)->next->txt;
	quotetype = find_first_quote(delimiter);
	if (quotetype)
		delimiter = ft_strtrim(delimiter, &quotetype);
	(*input)->hd_fd = here_child(delimiter, retval, quotetype, env);
	if ((*input)->hd_fd < 0)
		return (-1);
	signal(SIGINT, sigint_handler);
	return ((*input)->hd_fd);
}