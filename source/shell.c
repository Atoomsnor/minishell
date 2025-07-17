/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:09:11 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/18 01:45:44 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <stdio.h>

static char	*skip_spaces(char *in, int *skip)
{
	char	*ret;
	int		i;

	i = 0;
	if (!in)
		return (NULL);
	while (ft_iswhitespace(in[i]))
		i++;
	if (in[i])
	{
		ret = ft_substr(in, i, ft_strlen(in) - i);
		if (!ret)
			return (NULL);
		*skip = 1;
		return (ret);
	}
	free_and_null(in);
	return (NULL);
}

static void	check_write_error(t_exec **exec)
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

static int	shell_exec(int *retval, t_exec **exec,
						char ***envp, t_history *hist)
{
	if (exec[1] || (exec[0] && exec[0]->full_path && exec[0]->full_path[0] != '\0'))
	{
		*retval = execute(exec, *envp);
		check_write_error(exec);
		history(hist);
		return (die(exec, NULL, NULL, NULL), 0);
	}
	else if (exec[0] && exec[0]->full_cmd && exec[0]->full_cmd[0])
	{
		if (!ft_strncmp(exec[0]->full_cmd[0], "exit", 5))
			*retval = bi_exit(exec, envp, hist);
		else
			*retval = run_builtin(exec[0], exec[0]->out_fd, envp, 0);
		check_write_error(exec);
		if (!*envp)
			return (die(exec, NULL, NULL, set_retval(retval, -1)), 0);
		history(hist);
		return (die(exec, NULL, NULL, NULL), 0);
	}
	return (1);
}

static int	run(t_input **input, char ***envp, int retval, t_history *hist)
{
	t_exec	**exec;
	t_input	*head;

	exec = NULL;
	head = *input;
	exec = tokens_to_exec(input, *envp, &retval);
	*input = head;
	if (input && *input)
		shank_input(input);
	if (!exec)
		return (history(hist), retval);
	if (!shell_exec(&retval, exec, envp, hist))
		return (retval);
	if (exec)
		lynch_exec(exec);
	retval = 0;
	history(hist);
	return (retval);
}

int	shelly(char ***envp, int retval, t_history *hist)
{
	t_input	**input;
	char	*in;
	int		skipped_whitespace;

	skipped_whitespace = 0;
	in = readline("megashell>$ ");
	hist->in = in;
	if (g_signalreceived)
	{
		g_signalreceived = 0;
		retval = 130;
	}
	if (in == NULL)
		return (printf("exit\n"), -1);
	if (in && ft_iswhitespace(in[0]))
		in = skip_spaces(in, &skipped_whitespace);
	if (in && in[0] == '\0')
		return (free_and_null(in), retval);
	if (!in)
		return (retval);
	input = init_list(in, skipped_whitespace);
	if (!input)
		return (history(hist), 1);
	retval = run(input, envp, retval, hist);
	return (retval);
}
