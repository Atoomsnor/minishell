/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/05/15 17:51:19 by nhendrik         ###   ########.fr       */
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
	printf("in: %i\nout: %i\n", exec->in_fd, exec->out_fd);
	while (exec->full_cmd[i])
	{
		printf("%s\n", exec->full_cmd[i]);
		i++;
	}
	printf("execend\n");
}

int shelly(void)
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
		print_exec(*exec);
		if (exec[1])
			print_exec(exec[1]);
		for (int i = 0; exec[i]; i++)
		{
			if (exec[i]->in_fd > 1)
				close(exec[i]->in_fd);
			if (exec[i]->out_fd > 1)
				close(exec[i]->out_fd);
		}
		// expand_exec(); cat -> /usr/bin/cat etc etc
		// ^^ checkt ook builtins? -> if x == true run builtins else execution?
		// of voor expand_exec ?
		//execution
		
	}
	return (1);
}

int	main(void)
{
	init_signals();
	while (1)
	{
		if (!shelly())
			break ;
	}
}
