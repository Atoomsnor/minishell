/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:10:12 by roversch          #+#    #+#             */
/*   Updated: 2025/05/20 15:13:17 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int count_cmds(t_input *input)
{
	int count;

	count = 1;
	while (input)
	{
		if (input->type == t_pipe)
			count++;
		else if ((input->type == t_right || input->type == t_append)
				&& (input->next && input->next->next 
				&& (input->next->next->type == t_right 
				|| input->next->next->type == t_append)))
		{
			count++;
		}
		input = input->next;
	}
	return (count);
}

int count_till_pipe(t_input *input)
{
	t_input *cpy;
	int count;

	count = 0;
	cpy = input;
	while (cpy && (cpy->type == t_txt || cpy->type == t_flag))
	{
		count++;
		cpy = cpy->next;
	}
	return (count);
}

int find_in(t_input *input)
{
	while (input)
	{
		if (input->type == t_left)
			return (open(input->next->txt, O_RDONLY));
		else if (input->type == t_heredoc)
			return (open("./minishell", O_RDONLY));
		else if (input->type == t_pipe)
			return (0);
		if (input->prev)
			input = input->prev;
		else
			return (0);
	}
	return (0);
}

int find_out(t_input *input)
{
	while (input)
	{
		if (input->type == t_right)
			return (open(input->next->txt, O_WRONLY | O_CREAT | O_TRUNC, 0644));
		else if (input->type == t_append)
			return (open(input->next->txt, O_WRONLY | O_CREAT | O_APPEND, 0644));
		else if (input->type == t_pipe)
			return (1);
		input = input->next;
	}
	return (1);
}

t_exec *fill_exec(t_input **input)
{
	t_exec *cmd;
	int	count;
	int	i;

	i = 0;
	cmd = ft_calloc(1, sizeof(t_exec));
	if (!cmd)
		return (NULL);
	if ((*input)->type == t_right || (*input)->type == t_append)
	{
		if ((*input)->prev && (*input)->prev->prev && ((*input)->prev->prev->type == t_right || (*input)->prev->prev->type == t_append))
		{
			cmd->full_cmd = NULL;
			cmd->full_path = NULL;
			cmd->in_fd = 0;
			cmd->out_fd = find_out(*input);
			return (cmd);
		}
	}
	count = count_till_pipe(*input);
	cmd->full_cmd = ft_calloc(count + 1, sizeof(char *));
	if (!cmd->full_cmd)
		return (NULL);
	cmd->in_fd = find_in(*input);
	cmd->out_fd = find_out(*input);
	while ((*input) && ((*input)->type == t_txt || (*input)->type == t_flag))
	{
		if ((*input)->type == t_txt || (*input)->type == t_flag)
			cmd->full_cmd[i++] = ft_strdup((*input)->txt);
		*input = (*input)->next;
	}
	if (*input && !((*input)->type == t_txt || (*input)->type == t_flag))
		(*input) = (*input)->next;
	return (cmd);
}

int is_buildin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	return (0);
}

char *cmd_to_path(t_exec *cmd, t_exec **cmds)
{
	char *ret;

	ret = NULL;
	if (cmd->full_cmd[0][0] == '/')
	{
		if (access(cmd->full_cmd[0], F_OK | X_OK) == 0)
			return (cmd->full_cmd[0]);
		else
		 	return (die(cmds, NULL, error_input_fail), NULL); // error
	}
	if (is_buildin(cmd->full_cmd[0]))
		return (NULL);
	ret = find_path(split_paths(), cmd->full_cmd[0]);
	if (!ret)
		return (die(cmds, NULL, error_input_fail), NULL); // error
	return (ret);
}

t_exec **tokens_to_exec(t_input **input)
{
	t_exec	**cmds;
	int	count;
	int	i;

	count = count_cmds(*input);
	cmds = ft_calloc(count + 1, sizeof(t_exec *));
	i = 0;
	while (i < count)
	{
		while ((*input) && ((*input)->type == t_left || (*input)->type == t_heredoc))
			*input = (*input)->next->next;
		cmds[i] = fill_exec(input);
		cmds[i]->full_path = cmd_to_path(cmds[i], cmds);
		i++;
	}
	//remove quotes
	return (cmds);
}