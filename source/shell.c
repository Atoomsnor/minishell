/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:09:11 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/07 17:09:11 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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
	free(in);
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
	t_input	*head;
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
		exec = tokens_to_exec(input, *envp, &retval, hist);
		*input = head;
		if (input && *input)
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
