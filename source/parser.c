/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:10:12 by roversch          #+#    #+#             */
/*   Updated: 2025/07/08 17:35:25 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>

static void	fill_full_cmd(t_input **input, t_exec *cmd, int *i)
{
	if ((*input)->type == t_txt || (*input)->type == t_flag)
	{
		if ((*input)->prev && *i != 0)
		{
			if ((*input)->prev->type == t_txt
				|| (*input)->prev->type == t_flag)
				cmd->full_cmd[(*i)++] = ft_strdup((*input)->txt);
		}
		else
			cmd->full_cmd[(*i)++] = ft_strdup((*input)->txt);
	}
}

static t_exec	*fill_exec(t_input **input, char **error_msg)
{
	t_exec	*cmd;
	int		count;
	int		i;

	cmd = ft_calloc(1, sizeof(t_exec));
	if (!cmd)
		return (malloc_error_free(NULL));
	cmd->err_msg = NULL;
	count = count_till_pipe(*input);
	cmd->full_cmd = ft_calloc(count + 1, sizeof(char *));
	if (!cmd->full_cmd)
		return (malloc_error_free(free_and_null(cmd)));
	i = find_in_out(*input, &cmd->in_fd, &cmd->out_fd, error_msg);
	if (i < 0 && i != -2)
		return (free_and_null(cmd->full_cmd), free_and_null(cmd), NULL);
	else if (i == -2)
		return (free_and_null(cmd->full_cmd), free_and_null(cmd),
			ft_strmcpy(error_msg, " Permission denied\n"), NULL);
	while ((*input) && (*input)->type != t_pipe && i < count)
	{
		fill_full_cmd(input, cmd, &i);
		*input = (*input)->next;
	}
	return (cmd);
}

static int	prepare_input(t_input **input, char **envp, int *retval)
{
	rotate_input(input);
	if (!(*input))
		return (ft_putstr_fd("Parsing error\n", 2), 0);
	if (check_heredoc(*input, *retval, envp) == -1)
		return (0);
	input = dequote(envp, *retval, input);
	if (!input)
		return (0);
	input = check_empty_txt(input);
	if (!input)
		return (set_retval(retval, 0), 0);
	return (1);
}

static int	path_validity(t_exec *cmd, char **error_msg,
							int *retval, char **envp)
{
	if (!check_dir(cmd->full_cmd[0], error_msg, 0)
		|| !check_access(cmd->full_cmd[0], error_msg))
		return (set_retval(retval, 126), 0);
	cmd->full_path = cmd_to_path(cmd, error_msg, envp);
	if (!cmd->full_path)
		return (set_retval(retval, 127), 0);
	if (error_msg)
		cmd->err_msg = *error_msg;
	return (1);
}

t_exec	**tokens_to_exec(t_input **input, char **envp, int *retval)
{
	t_exec		**cmds;
	char		*error_msg;
	int			count;
	int			i;

	error_msg = NULL;
	count = count_cmds(*input);
	cmds = ft_calloc(count + 1, sizeof(t_exec *));
	if (!cmds)
		return (malloc_error_free(NULL));
	i = 0;
	while (i < count)
	{
		if (!prepare_input(input, envp, retval))
			return (lynch_exec(cmds), NULL);
		cmds[i] = fill_exec(input, &error_msg);
		if (!cmds[i] && !has_type(*input, t_pipe))
			return (die(cmds, NULL, error_msg, set_retval(retval, 1)));
		else if (!cmds[i])
			count = rotate_past_pipe(input, count);
		else
		{
			if (!path_validity(cmds[i++], &error_msg, retval, envp))
				return (die(cmds, NULL, error_msg, NULL));
		}
	}
	return (cmds);
}
