/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/05/20 17:15:36 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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
	int		returnvalue;
	t_exec	**exec;
	char	*in;
	t_input **input;

	exec = NULL;
	returnvalue = 1;
	in = readline("megashell>$ ");
	if (g_signalreceived)
	{
		g_signalreceived = 0;
		returnvalue = 130;
	}
	if (in == NULL)
		return (0);
	if (in[0] != '\0')
	{
		history(in);
		input = init_list(in);
		exec = tokens_to_exec(input);
		if (!exec)
			return (1);
		execute(exec, envp);
	}
	return (returnvalue);
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
