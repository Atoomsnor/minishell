/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:10:12 by roversch          #+#    #+#             */
/*   Updated: 2025/06/09 13:13:07 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

char	**split_paths(void)
{
	char	**paths;
	char	*tmp;
	char	*path_env;
	int		i;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (free_array(paths), NULL);
		free(paths[i]);
		paths[i] = tmp;
		i++;
	}
	return (paths);
}

char	*find_path(char **paths, char *cmd)
{
	char	*found_path;
	int		i;

	if (!cmd || !paths)
		return (NULL);
	if (access(cmd, F_OK | X_OK) == 0)
		return (cmd);
	i = 0;
	while (paths[i])
	{
		found_path = ft_strjoin(paths[i], cmd);
		if (!found_path)
			return (NULL);
		if (access(found_path, F_OK | X_OK) == 0)
			return (found_path);
		free(found_path);
		i++;
	}
	return (NULL);
}

int	count_cmds(t_input *input)
{
	int	count;

	count = 1;
	while (input)
	{
		if (input->type == t_pipe && input->next && input->next->type != t_right
			&& input->next->type != t_append)
			count++;
		input = input->next;
	}
	return (count);
}

int	count_till_pipe(t_input *input)
{
	t_input	*cpy;
	int		count;

	count = 0;
	cpy = input;
	while (cpy && (cpy->type == t_txt || cpy->type == t_flag))
	{
		count++;
		cpy = cpy->next;
	}
	return (count);
}

int	find_in(t_input *input)
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

int	find_out(t_input *input)
{
	int	fd;

	fd = 1;
	while (input)
	{
		if (input->type == t_right)
		{
			if (fd > 1)
				close(fd);
			fd = open(input->next->txt, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		else if (input->type == t_append)
		{
			if (fd > 1)
				close(fd);
			fd = open(input->next->txt, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else if (input->type == t_pipe)
			return (fd);
		input = input->next;
	}
	return (fd);
}

t_exec	*fill_exec(t_input **input)
{
	t_exec	*cmd;
	int		pipe_fd[2];
	int		count;
	int		i;

	i = 0;
	cmd = ft_calloc(1, sizeof(t_exec));
	if (!cmd)
		return (NULL);
	count = count_till_pipe(*input);
	cmd->full_cmd = ft_calloc(count + 1, sizeof(char *));
	if (!cmd->full_cmd)
		return (free(cmd), NULL);
	cmd->in_fd = find_in(*input);
	if (cmd->in_fd < 0)
		return (NULL);
	cmd->out_fd = find_out(*input);
	if (cmd->out_fd < 0)
		return (NULL);
	cmd->pipe[0] = pipe_fd[0];
	cmd->pipe[1] = pipe_fd[1];
	while ((*input) && ((*input)->type == t_txt || (*input)->type == t_flag))
	{
		if ((*input)->type == t_txt || (*input)->type == t_flag)
			cmd->full_cmd[i] = ft_strdup((*input)->txt);
		i++;
		*input = (*input)->next;
	}
	return (cmd);
}

int	is_buildin(char *cmd)
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

char	*cmd_to_path(t_exec *cmd)
{
	char	**paths;
	char	*ret;

	ret = NULL;
	if (cmd->full_cmd[0][0] == '/')
	{
		if (access(cmd->full_cmd[0], F_OK | X_OK) == 0)
			return (cmd->full_cmd[0]);
		else
			return (NULL);
	}
	if (is_buildin(cmd->full_cmd[0]))
		return ("");
	paths = split_paths();
	if (!paths)
		return (NULL);
	ret = find_path(paths, cmd->full_cmd[0]);
	free_array(paths);
	return (ret);
}

void check_heredoc(t_input *input)
{
	while (input)
	{
		if (input->type == t_heredoc)
			// run_heredoc();
		;
		else if (input->type == t_pipe)
			return ;
		if (input->prev)
			input = input->prev;
		else
			return ;
	}
	return ;
}

t_exec	**tokens_to_exec(t_input **input, char **envp, int retval)
{
	t_exec	**cmds;
	t_input	*head;
	int		count;
	int		i;

	count = count_cmds(*input);
	cmds = ft_calloc(count + 1, sizeof(t_exec *));
	i = 0;
	head = *input;
	while (i < count)
	{
		while (*input)
		{
			if (((*input)->type == t_flag || (*input)->type == t_txt) && (*input)->prev && ((*input)->prev->type == t_left || (*input)->prev->type == t_heredoc) &&  (*input)->next && ((*input)->next->type != t_right || (*input)->next->type == t_append))
				(*input) = (*input)->next;
			else if ((*input) && (*input)->next && ((*input)->type == t_append || (*input)->type == t_right) && !(*input)->next->next)
			{
				if ((*input)->prev)
					(*input) = (*input)->prev;
				break ;
			}
			else if ((*input)->type == t_flag || (*input)->type == t_txt)
				break ;
			else
				(*input) = (*input)->next;
		}
		if (!(*input))
			return (die(cmds, input, error_fill_exec), NULL);
		check_heredoc(*input);
		input = dequote(envp, retval, input);
		cmds[i] = fill_exec(input);
		if (!cmds[i])
			return (die(cmds, input, error_fill_exec), NULL);
		if (cmds[i]->full_cmd)
		{
			cmds[i]->full_path = cmd_to_path(cmds[i]);
			if (!cmds[i]->full_path)
				return (die(cmds, input, error_cmd_to_path), NULL);
		}
		i++;
	}
	*input = head;
	return (cmds);
}
