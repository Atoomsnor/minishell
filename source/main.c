/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/06/25 20:53:03 by nhendrik         ###   ########.fr       */
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

void check_write_error(t_exec **exec)
{
	int	i;

	i = 0;
	while (exec[i])
	{
		if (exec[i]->err_msg)
			ft_putstr_fd(exec[i]->err_msg, 2);
		i++;
	}
}

int	shelly(char ***envp, int retval, char **hist)
{
	t_input	**input;
	t_exec	**exec;
	t_input	*head;
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
		// printlist(*input, 0);
		head = *input;
		exec = tokens_to_exec(input, *envp, &retval, hist);
		if (!exec)
			return (retval);
		*input = head;
		shank_input(input);
		// print_exec(exec[0]);
		// if (exec[1])
			// print_exec(exec[1]);
		if (exec[1] || exec[0]->full_path[0] != '\0')
		{
			retval = execute(exec, *envp);
			check_write_error(exec);
			return (die(exec, NULL, NULL), retval);
		}
		else
		{
			retval = run_builtin(exec[0], exec[0]->out_fd, envp, 0);
			check_write_error(exec);
			return (die(exec, NULL, NULL), retval);
		}
		if (exec)
			lynch_exec(exec);
		retval = 0;
		//save_history(NULL, 1, hist);
	}
	return (retval);
}

char	**ft_matdup(char **mat)
{
	char	**cpy;
	int		len;

	if (!mat)
		return (NULL);
	len = 0;
	while (mat[len])
		len++;
	cpy = ft_calloc(len + 1, sizeof(char *));
	len = 0;
	while(mat[len])
	{
		cpy[len] = ft_strdup(mat[len]);
		len++;
	}
	return (cpy);
}

int	main(int argc, char **argv, char **envp)
{
	char	*hist[HISTORY_SIZE];
	char	**environment;
	int		retval;


	environment = ft_matdup(envp);
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
