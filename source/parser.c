/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:10:12 by roversch          #+#    #+#             */
/*   Updated: 2025/07/03 17:46:05 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

int	file_is_empty(char *path)
{
	int		fd;
	char	buff[2];

	if (!path)
		return (1);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (1);
	if (read(fd, buff, 2) == 0)
		return (close(fd), 1);
	close(fd);
	return (0);
}

t_exec	*fill_exec(t_input **input, char **error_msg)
{
	t_exec	*cmd;
	int		count;
	int		i;

	cmd = ft_calloc(1, sizeof(t_exec));
	if (!cmd)
		return (NULL);
	count = count_till_pipe(*input);
	cmd->full_cmd = ft_calloc(count + 1, sizeof(char *));
	if (!cmd->full_cmd)
		return (free(cmd), NULL);
	i = find_in_out(*input, &cmd->in_fd, &cmd->out_fd, error_msg);
	if (i < 0 && i != -2)
		return (free_and_null(cmd->full_cmd), free_and_null(cmd), NULL);
	else if (i == -2)
		return (free_and_null(cmd->full_cmd), free_and_null(cmd),
			ft_strmcpy(error_msg, " Permission denied\n"), NULL);
	while ((*input) && (*input)->type != t_pipe && i < count)
	{
		if ((*input)->type == t_txt || (*input)->type == t_flag)
		{
			if ((*input)->prev && i != 0)
			{
				if ((*input)->prev->type == t_txt
					|| (*input)->prev->type == t_flag)
					cmd->full_cmd[i++] = ft_strdup((*input)->txt);
			}
			else
				cmd->full_cmd[i++] = ft_strdup((*input)->txt);
		}
		*input = (*input)->next;
	}
	return (cmd);
}

t_exec	**tokens_to_exec(t_input **input, char **envp,
				int *retval, t_history *hist)
{
	t_exec	**cmds;
	char	*error_msg;
	int		count;
	int		i;

	count = count_cmds(*input);
	cmds = ft_calloc(count + 1, sizeof(t_exec *));
	i = 0;
	error_msg = NULL;
	while (i < count)
	{
		rotate_input(input);
		if (!(*input))
			return (die(cmds, NULL, ft_strdup("Parsing error\n"), NULL));
		check_heredoc(*input, hist, *retval, envp);
		input = dequote(envp, *retval, input);
		if (!input)
			return (free(cmds), NULL);
		input = check_empty_txt(input);
		if (!input)
			return (lynch_exec(cmds), set_retval(retval, 0), NULL);
		cmds[i] = fill_exec(input, &error_msg);
		if (!cmds[i] && !has_type(*input, t_pipe))
			return (die(cmds, NULL, error_msg, set_retval(retval, 1)));
		else if (!cmds[i])
			count = rotate_past_pipe(input, count);
		else
		{
			if (!check_dir(cmds[i]->full_cmd[0], &error_msg, 0)
				|| !check_access(cmds[i]->full_cmd[0], &error_msg))
				return (die(cmds, NULL, error_msg, set_retval(retval, 126)));
			cmds[i]->full_path = cmd_to_path(cmds[i], &error_msg, envp);
			if (!cmds[i]->full_path)
				return (die(cmds, NULL, error_msg, set_retval(retval, 127)));
			if (error_msg)
				cmds[i]->err_msg = error_msg;
			i++;
		}
	}
	if (!cmds[0])
		return (free(cmds), NULL);
	return (cmds);
}
