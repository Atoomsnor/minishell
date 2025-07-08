/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:16:34 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/08 17:08:38 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	here_child(char *delimiter, int pipefd,
							char **env, int retval)
{
	char	*input;
	char	quotetype;

	signal(SIGINT, SIG_DFL);
	quotetype = find_first_quote(delimiter);
	if (quotetype)
		delimiter = ft_strtrim(delimiter, &quotetype);
	while (1)
	{
		input = readline("> ");
		if (!input)
			break ;
		if (!ft_strncmp(input, delimiter, ft_strlen(delimiter)))
		{
			free(input);
			break ;
		}
		if (has_char(input, '$') >= 0 && quotetype != '\'')
			input = handle_wildcard(input, env, retval, 0);
		write(pipefd, input, ft_strlen(input));
		write(pipefd, "\n", 1);
		free(input);
	}
	exit(1);
}

static int	here_parent(char *delimiter, int retval, char **env)
{
	pid_t	pid;
	int		status;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		close(pipefd[0]);
		here_child(delimiter, pipefd[1], env, retval);
	}
	signal(SIGINT, SIG_IGN);
	close(pipefd[1]);
	wait(&status);
	if (status == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (close(pipefd[0]), -1);
	}
	if (status >> 8 != 1)
		return (close(pipefd[0]), -1);
	return (pipefd[0]);
}

static int	run_here_doc(t_input **input, t_history *hist,
							int retval, char **env)
{
	char	*delimiter;

	(void)hist;
	delimiter = (*input)->next->txt;
	(*input)->hd_fd = here_parent(delimiter, retval, env);
	signal(SIGINT, sigint_handler);
	if ((*input)->hd_fd < 0)
		return (-1);
	return ((*input)->hd_fd);
}

int	check_heredoc(t_input *input, t_history *hist, int retval, char **env)
{
	while (input && input != input->head
		&& input->type != t_pipe && input->prev)
		input = input->prev;
	while (input && input->type != t_pipe)
	{
		if (input->type == t_heredoc && input->hd_fd == 0)
			input->hd_fd = run_here_doc(&input, hist, retval, env);
		else if (input->type == t_pipe)
			return (0);
		if (input->hd_fd == -1)
			return (-1);
		input = input->next;
	}
	return (0);
}
