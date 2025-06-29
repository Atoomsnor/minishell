/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:10:12 by roversch          #+#    #+#             */
/*   Updated: 2025/06/27 17:43:26 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

int	check_dir(char *str, char **error_msg, int i)
{
	DIR *dir;

	if (!str)
		return (0);
	if (i || str[0] == '/' || !ft_strncmp(str, "./", 2))
	{
		dir = opendir(str);
		if (dir)
		{
			closedir(dir);
			if (error_msg)
				*error_msg = ft_strjoin(str, ": Is a directory\n");
			return (0);
		}
	}
	return (1);
}

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
	if (!check_dir(cmd, NULL, 1))
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
		return (die(NULL, NULL, " Permission denied\n", NULL), NULL);
	cmd->out_fd = find_out(*input);
	if (cmd->out_fd < 0 && cmd->out_fd != -2)
		return (NULL);
	else if (cmd->out_fd == -2)
		return ((die(NULL, NULL, " Permission denied\n", NULL), NULL));
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

void *adjust_error(char **error_msg, char *err1, char *err2)
{
	if (err1)
		*error_msg = ft_strjoin(*error_msg, err1);
	if (err2)
		*error_msg = ft_strjoin(*error_msg, err2);
	return (NULL);
}

int is_executable_script(char *path)
{
	char	buff[4];
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	if (read(fd, buff, 4) < 4)
		return (close(fd), 1);
	close(fd);
	if (!ft_strncmp(buff, "#!", 2) || (buff[0] == 0x7f && !ft_strncmp(&buff[1], "ELF", 3)))
		return (1);
	else
		return (0);
}

char	*cmd_to_path(t_exec *cmd, char **error_msg)
{
	char	**paths;
	char	*ret;

	ret = NULL;
	if (cmd->full_cmd[0][0] == '/')
	{
		if (access(cmd->full_cmd[0], F_OK | X_OK) == 0)
			return (cmd->full_cmd[0]);
		else
			return (adjust_error(error_msg, cmd->full_cmd[0], ": No such file or directory\n"));
	}
	if (is_buildin(cmd->full_cmd[0]))
		return ("");
	paths = split_paths();
	if (!paths)
		return (adjust_error(error_msg, cmd->full_cmd[0], ": command not found\n"));
	ret = find_path(paths, cmd->full_cmd[0]);
	free_array(paths);
	if (!ret || !is_executable_script(ret))
		return (adjust_error(error_msg, cmd->full_cmd[0], ": command not found\n"));
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

void *set_retval(int *retval, int val)
{
	*retval = val;
	return (NULL);
}

void ft_lstcopy(t_input *input, t_input *next)
{
	input->hd_fd = next->hd_fd;
	if (input->txt)
		free(input->txt);
	input->txt = ft_strdup(next->txt);
	input->type = next->type;
}

t_input	**check_empty_txt(t_input **input)
{
	t_input *head;
	t_input *next;

	head = *input;
	while (*input)
	{
		next = (*input)->next;
		if ((*input)->txt && (*input)->txt[0] == '\0')
		{
			if (*input == head && next)
			{
				ft_lstcopy(*input, (*input)->next);
				next = *input;
				ft_lstdelone((*input)->next);
				
			}
			else if (*input == head && !next)
			{
				shank_input(input);
				return (NULL);
			}
			else
				ft_lstdelone(*input);
		}
		*input = next;
	}
	*input = head;
	if (!input || !(*input))
		return (NULL);
	return (input);
}

int file_is_empty(char *path)
{
	int	fd;
	char buff[2];

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
			return (die(cmds, input, "rotation error\n", NULL));
		check_heredoc(*input, hist);
		input = dequote(envp, *retval, input);
		if (!input)
			return (free(cmds), NULL);
		input = check_empty_txt(input);
		if (!input)
			return (free(cmds), NULL);
		cmds[i] = fill_exec(input, &error_msg);
		if (!cmds[i] && !has_type(*input, t_pipe))
			return (die(cmds, input, error_msg, set_retval(retval, 1)));
		else if (!cmds[i])
			count = rotate_past_pipe(input, count);
		else
		{
			if (!check_dir(cmds[i]->full_cmd[0], &error_msg, 0))
				return (die(cmds, input, error_msg, set_retval(retval, 126)));
			cmds[i]->full_path = cmd_to_path(cmds[i], &error_msg);
			if (!cmds[i]->full_path)
				return (die(cmds, input, error_msg, set_retval(retval, 127))); // fix so die prints correct error
			if (file_is_empty(cmds[i]->full_path))
				return (printf("pure death\n"), die(cmds, input, NULL, set_retval(retval, 0)));
			if (error_msg)
				cmds[i]->err_msg = error_msg;
			i++;
		}
	}
	if (!cmds[0])
		return (free(cmds), NULL);
	return (cmds);
}
