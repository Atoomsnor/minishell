/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:10:12 by roversch          #+#    #+#             */
/*   Updated: 2025/06/27 13:36:38 by roversch         ###   ########.fr       */
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
	while (cpy && (cpy->type != t_pipe))
	{
		if ((cpy->type == t_append || cpy->type == t_right
				|| cpy->type == t_heredoc || cpy->type == t_left)
					&& cpy->next && cpy->next->next)
			cpy = cpy->next->next;
		else if (cpy->type == t_txt || cpy->type == t_flag)
		{
			count++;
			cpy = cpy->next;
		}
		else
			return (count);
	}
	return (count);
}

int	find_in(t_input *input)
{
	int	fd;

	fd = 0;
	while (input)
	{
		if (input->type == t_left)
		{
			if (access(input->next->txt, F_OK) != 0)
				return (-1);
			if (access(input->next->txt, R_OK) != 0)
				return (-2);
			fd = open(input->next->txt, O_RDONLY);
		}
		else if (input->type == t_heredoc)
			fd = input->hd_fd;
		if (input->prev && input->prev->type != t_pipe)
			input = input->prev;
		else
			break ;
		if (fd < 0)
			return (-1);
	}
	while (input)
	{
		if (input->type == t_left)
		{
			if (access(input->next->txt, F_OK) != 0)
				return (-1);
			if (access(input->next->txt, R_OK) != 0)
				return (-2);
			fd = open(input->next->txt, O_RDONLY);
		}
		else if (input->type == t_heredoc)
			fd = input->hd_fd;
		else if (input->type == t_pipe)
			break ;
		if (fd < 0)
			return (-1);
		input = input->next;
	}
	return (fd);
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
			if (access(input->next->txt, F_OK) == 0)
				if (access(input->next->txt, W_OK) != 0)
					return (-2);
			fd = open(input->next->txt, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		else if (input->type == t_append)
		{
			if (fd > 1)
				close(fd);
			if (access(input->next->txt, F_OK) == 0)
				if (access(input->next->txt, W_OK) != 0)
					return (-2);
			fd = open(input->next->txt, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else if (input->type == t_pipe)
			return (fd);
		input = input->next;
	}
	return (fd);
}

t_exec	*fill_exec(t_input **input, char **error_msg)
{
	t_exec	*cmd;
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
	if (cmd->in_fd == -1)
		return (ft_strmcpy(error_msg, " No such file or directory\n"), NULL);
	else if (cmd->in_fd == -2)
		return (die(NULL, NULL, " Permission denied\n"), NULL);
	cmd->out_fd = find_out(*input);
	if (cmd->out_fd < 0 && cmd->out_fd != -2)
		return (NULL);
	else if (cmd->out_fd == -2)
		return ((die(NULL, NULL, " Permission denied\n"), NULL));
	while ((*input) && (*input)->type != t_pipe && i < count)
	{
		if ((*input)->type == t_txt || (*input)->type == t_flag)
		{
			if ((*input)->prev && i != 0)
			{
				if ((*input)->prev->type == t_txt || (*input)->prev->type == t_flag)
					cmd->full_cmd[i++] = ft_strdup((*input)->txt);
			}
			else
				cmd->full_cmd[i++] = ft_strdup((*input)->txt);
		}
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

void	check_heredoc(t_input *input, char **hist)
{
	t_input	*curr;

	curr = input;
	(void)hist;
	while (input && input != input->head
		&& input->type != t_pipe && input->prev)
		input = input->prev;
	while (input && input != curr)
	{
		if (input->type == t_heredoc && input->hd_fd == 0)
			input->hd_fd = run_here_doc(&input, input->next->txt, hist);
		else if (input->type == t_pipe)
			return ;
		input = input->next;
	}
	return ;
}

int	has_type(t_input *input, t_type type)
{
	t_input	*cpy;

	cpy = input;
	while (cpy)
	{
		if (cpy->type == type)
			return (1);
		cpy = cpy->next;
	}
	return (0);
}

void	rotate_input(t_input **input)
{
	while (*input)
	{
		// if (((*input)->type == t_flag || (*input)->type == t_txt) && (((*input)->prev && ((*input)->prev->type == t_left || (*input)->prev->type == t_heredoc)) && ((*input)->next && ((*input)->next->type == t_right || (*input)->next->type == t_append))))
		// 	(*input) = (*input)->next;
		if ((*input) && (*input)->next && ((*input)->type == t_append
				|| (*input)->type == t_right) && !(*input)->next->next)
		{
			if ((*input)->prev)
				(*input) = (*input)->prev;
			return ;
		}
		else if (((*input)->type == t_flag || (*input)->type == t_txt)
			&& (*input)->prev && ((*input)->prev->type == t_left
				|| (*input)->prev->type == t_heredoc))
			(*input) = (*input)->next;
		else if (((*input)->type == t_flag || (*input)->type == t_txt)
			&& (*input)->prev && ((*input)->prev->type == t_right
				|| (*input)->prev->type == t_append))
			(*input) = (*input)->next;
		else if ((*input)->type == t_flag || (*input)->type == t_txt)
			return ;
		else
			(*input) = (*input)->next;
	}
}

int	rotate_past_pipe(t_input **input, int count)
{
	while (*input && (*input)->type != t_pipe)
		*input = (*input)->next;
	if (*input && (*input)->type == t_pipe)
		*input = (*input)->next;
	return (count - 1);
}

t_exec	**tokens_to_exec(t_input **input, char **envp, int *retval, char **hist)
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
			return (die(cmds, input, "error fill exec\n"), NULL);
		check_heredoc(*input, hist);
		input = dequote(envp, *retval, input);
		if (!input)
			return (free(cmds), NULL);
		cmds[i] = fill_exec(input, &error_msg);
		if (!cmds[i] && !has_type(*input, t_pipe))
		{
			*retval = 1;
			return (die(cmds, input, error_msg), NULL);
		}
		else if (!cmds[i])
			count = rotate_past_pipe(input, count);
		else
		{
			cmds[i]->full_path = cmd_to_path(cmds[i]);
			if (!cmds[i]->full_path)
				return (die(cmds, input, ft_strjoin(cmds[i]->full_cmd[0], " : command not found\n")), NULL); // fix so die prints correct error
			if (error_msg)
				cmds[i]->err_msg = error_msg;
			i++;
		}
	}
	return (cmds);
}
