/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/05/15 14:34:29 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

// void file_handler_prep(t_shell *shell)
// {
// 	t_input	*curr;
// 	t_input *next;

// 	curr = *shell->curr_input;
// 	while (curr->next)
// 	{
// 		next = curr->next;
// 		if (curr->type != t_txt && curr->type != t_flag)
// 			ft_lstdelone(curr);
// 		else if (curr->type == t_flag)
// 		{
// 			ft_strjoin(curr->txt, next->txt);
// 			ft_lstdelone(next);
// 			next = curr->next;
// 		}
// 		curr = next;
// 	}
// 	*shell->curr_input = curr->head;
// 	printlist(*shell->curr_input);
// 	file_handler(shell);
// }

int shelly(char **envp)
{
	t_exec	**exec;
	char	*in;
	t_input **input;

	exec = NULL;
	in = readline("megashell>$ ");
	if (in == NULL)
		return (0);
	if (in[0] != '\0')
	{
		history(in);
		input = init_list(in);
		exec = tokens_to_exec(input);
		//excecution

		// if (has_type(*shell->curr_input, t_left, t_right, t_heredoc, t_append, 0))
		// 	execute_cmds(shell);
		// else
		// 	singlecmd((*shell->curr_input)->txt, shell->envp);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{

	(void)argc;
	(void)argv;
	init_signals();
	while (1)
	{
		if (!shelly(envp))
			break ;
	}
}
