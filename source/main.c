/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/05/15 14:28:40 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void	history(t_shell *shell)
{
	static char	*hist[HISTORY_SIZE];
	static int	hist_count;
	int			i;

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
	t_exec **exec;

	exec = NULL;
	shell->in = readline("megashell>$ ");
	if (shell->in == NULL)
		return (0);
	if (shell->in[0] != '\0')
	{
		history(shell);
		shell->curr_input = init_list(shell);
		exec = tokens_to_exec(shell);
		//excecution

		// if (has_type(*shell->curr_input, t_left, t_right, t_heredoc, t_append, 0))
		// 	execute_cmds(shell);
		// else
		// 	singlecmd((*shell->curr_input)->txt, shell->envp);
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
	{
		if (!shelly(&shell))
			break ;
	}
}
