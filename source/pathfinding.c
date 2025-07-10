/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:22:43 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/10 13:35:10 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

static int	is_executable_script(char *path)
{
	char	buff[4];
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	if (read(fd, buff, 4) < 4)
		return (close(fd), 1);
	close(fd);
	if (!ft_strncmp(buff, "#!", 2)
		|| (buff[0] == 0x7f && !ft_strncmp(&buff[1], "ELF", 3)))
		return (1);
	else
		return (0);
}

static char	**split_paths(char **envp)
{
	char	**paths;
	char	*tmp;
	int		i;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
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

static char	*find_path(char **paths, char *cmd)
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

char	*cmd_to_path(t_exec *cmd, char **error_msg, char **envp)
{
	char	**paths;
	char	*ret;

	ret = NULL;
	if (cmd->full_cmd[0][0] == '/' || !ft_strncmp(cmd->full_cmd[0], "./", 2)
		|| !ft_strncmp(cmd->full_cmd[0], "../", 3))
	{
		if (access(cmd->full_cmd[0], F_OK | X_OK) == 0)
			return (ft_strdup(cmd->full_cmd[0]));
		else
			return (adjust_error(error_msg, cmd->full_cmd[0],
					": No such file or directory\n"));
	}
	if (is_buildin(cmd->full_cmd[0]))
		return ("");
	paths = split_paths(envp);
	if (!paths)
		return (adjust_error(error_msg, cmd->full_cmd[0],
				": command not found\n"));
	ret = find_path(paths, cmd->full_cmd[0]);
	free_array(paths);
	if (!ret || !is_executable_script(ret))
		return (adjust_error(error_msg, cmd->full_cmd[0],
				": command not found\n"));
	return (ret);
}
