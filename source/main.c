/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/07/03 17:17:07 by nhendrik         ###   ########.fr       */
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

void	check_write_error(t_exec **exec)
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

int	shelly(char ***envp, int retval, t_history *hist)
{
	t_input	**input;
	t_exec	**exec;
	t_input *head;
	char	*in;

	exec = NULL;
	in = readline("megashell>$ ");
	hist->in = in;
	if (g_signalreceived)
	{
		g_signalreceived = 0;
		retval = 130;
	}
	if (in == NULL)
		return (printf("exit\n"), -1);
	if (in && in[0] == ' ')
		in = skip_spaces(in);
	if (in && in[0] != '\0')
	{
		input = init_list(in);
		if (!input)
			return (1);
		head = *input;
		// printlist(head, 0);
		exec = tokens_to_exec(input, *envp, &retval, hist);
		if (input)
			*input = head;
		// printf("%s\n", (*input)->txt);
		if (input && *input && (*input)->txt)
			shank_input(input);
		if (!exec)
			return (history(hist), retval);
		if (exec[1] || exec[0]->full_path[0] != '\0')
		{
			retval = execute(exec, *envp);
			check_write_error(exec);
			history(hist);
			return (die(exec, NULL, NULL, NULL), retval);
		}
		else
		{
			retval = run_builtin(exec[0], exec[0]->out_fd, envp, 0);
			check_write_error(exec);
			history(hist);
			return (die(exec, NULL, NULL, NULL), retval);
		}
		if (exec)
			lynch_exec(exec);
		retval = 0;
		history(hist);
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
	while (mat[len])
	{
		cpy[len] = ft_strdup(mat[len]);
		len++;
	}
	return (cpy);
}

t_history *init_hist()
{
	t_history *hist;

	hist = ft_calloc(1, sizeof(t_history));
	hist->hist_count = 0;
	hist->in = NULL;
	return (hist);
}

int	main(int argc, char **argv, char **envp)
{
	t_history *hist;
	char	**environment;
	int		retval;

	environment = ft_matdup(envp);
	(void)argc;
	(void)argv;
	retval = 0;
	hist = init_hist();
	init_signals();
	while (1)
	{
		retval = shelly(&environment, retval, hist);
		if (retval == -1)
			break ;
	}
	free_array(environment);
	burn_history(hist);
}
