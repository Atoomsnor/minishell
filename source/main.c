/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/06/18 18:47:24 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void	print_exec(t_exec *exec)
{
	int	i;

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

char	*skip_spaces(char *in)
{
	char	*ret;
	int		i;

	i = 0;
	if (!in)
		return (NULL);
	while (in[i] == ' ')
		i++;
	if (in[i])
	{
		ret = ft_substr(in, i, ft_strlen(in) - i);
		free(in);
		return (ret);
	}
	return (NULL);
}

int	shelly(char ***envp, int retval, char **hist)
{
	t_input	**input;
	t_exec	**exec;
	char	*in;

	exec = NULL;
	in = readline("megashell>$ ");
	if (g_signalreceived)
	{
		g_signalreceived = 0;
		retval = 130;
	}
	if (in == NULL)
		return (-1);
	if (in && in[0] == ' ')
		in = skip_spaces(in);
	if (in && in[0] != '\0')
	{
		//save_history(in, 0, hist);
		input = init_list(in);
		if (!input)
			return (1);
		exec = tokens_to_exec(input, *envp, retval, hist);
		if (!exec)
			return (0);
		shank_input(input);
		if (exec[1] || exec[0]->full_path[0] != '\0')
			execute(exec, *envp);
		else
		{
			if (!run_builtin(exec[0], exec[0]->out_fd, envp, 0))
				return (die(exec, NULL, NULL), 1);
		}
			
		if (exec)
			lynch_exec(exec);
		retval = 0;
		//save_history(NULL, 1, hist);
	}
	return (retval);
}


int	main(int argc, char **argv, char **envp)
{
	char	*hist[HISTORY_SIZE];
	char	**environment;
	int		retval;


	environment = envp; //TODO matrix copy function
	(void)argc;
	(void)argv;
	retval = 0;
	init_signals();
	while (1)
	{
		retval = shelly(&environment, retval, hist);
		if (retval == -1)
			break ;
	}
	rl_clear_history();
}
