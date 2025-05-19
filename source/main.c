/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/05/19 17:31:31 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
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

void print_exec(t_exec *exec)
{
	int i;

	i = 0;
	printf("exec\n");
	printf("in: %i\nout: %i\nfullpath: %s\n", exec->in_fd, exec->out_fd, exec->full_path);
	while (exec->full_cmd[i])
	{
		printf("%s\n", exec->full_cmd[i]);
		i++;
	}
	printf("execend\n");
}

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
		// print_exec(*exec);
		// if (exec[1])
			// print_exec(exec[1]);
		execute(exec, envp);
		// for (int i = 0; exec[i]; i++)
		// {
		// 	if (exec[i]->in_fd > 1)
		// 		close(exec[i]->in_fd);
		// 	if (exec[i]->out_fd > 1)
		// 		close(exec[i]->out_fd);
		// }
		
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
