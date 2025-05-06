/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/04/29 17:48:07 by nhendrik         ###   ########.fr       */
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

void	history(t_shell *shell)
{
	int	i;

	i = 1;
	if (shell->hist_count < HISTORY_SIZE)
	{
		shell->hist[shell->hist_count] = shell->in;
		add_history(shell->in);
		shell->hist_count++;
	}
	else
	{
		rl_clear_history();
		while (i < HISTORY_SIZE)
		{
			shell->hist[i - 1] = shell->hist[i];
			add_history(shell->hist[i - 1]);
			i++;
		}
		shell->hist[HISTORY_SIZE - 1] = shell->in;
		add_history(shell->in);
	}
}

void file_handler_prep(t_shell *shell)
{
	t_input	*curr;
	t_input *next;

	curr = *shell->curr_input;
	while (curr->next)
	{
		next = curr->next;
		if (curr->type != t_txt && curr->type != t_flag)
			ft_lstdelone(curr);
		else if (curr->type == t_flag)
		{
			ft_strjoin(curr->txt, next->txt);
			ft_lstdelone(next);
			next = curr->next;
		}
		curr = next;
	}
	*shell->curr_input = curr->head;
	printlist(*shell->curr_input);
	file_handler(shell);
}

int shelly(t_shell *shell)
{
	shell->in = readline("megashell>$ ");
	if (shell->in && shell->in[0] != '\0')
	{
		history(shell);
		shell->curr_input = init_list(shell);
		if (has_type(*shell->curr_input, t_left, t_right, t_heredoc, t_append, 0))
			file_h(shell);
		else
			singlecmd((*shell->curr_input)->txt, shell->envp);
		if (g_signalreceived == SIGUSR1)
			g_signalreceived = 0;
	}
	return (1);
}

t_shell init_shell(char **envp)
{
	t_shell shell;

	shell.curr_input = NULL;
	shell.in = NULL;
	shell.envp = envp;
	ft_bzero(shell.hist, HISTORY_SIZE);
	shell.hist_count = 0;
	return (shell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell shell;

	(void)argc;
	(void)argv;
	init_signals();
	shell = init_shell(envp);
	while (1)
		shelly(&shell);
}
