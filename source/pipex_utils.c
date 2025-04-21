/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:38:04 by roversch          #+#    #+#             */
/*   Updated: 2025/04/21 12:27:03 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void	die(t_px *px, t_fd *fd, const char *msg, int exit_code)
{
	if (fd)
	{
		close(fd->in);
		close(fd->out);
		close(fd->pipe[0]);
		close(fd->pipe[1]);
	}
	free_array(px->paths);
	perror(msg);
	// printf("%i\n", exit_code);
	// kill(getpid(), SIGUSR1);
	exit(exit_code);
}

void	build_structs(t_px *px, t_fd *fd, int argc, char **argv)
{
	px->argc = argc;
	px->argv = argv;
	px->paths = split_paths(px->envp);
	if (!px->paths)
		die(px, NULL, "path error", 1);
	if (!ft_strncmp("here_doc", argv[0], 9))
		fd->in = open("./minishell", O_RDONLY);
	else
		fd->in = open(argv[0], O_RDONLY);
	if (fd->in == -1)
		die(px, fd, "infile error", 1);
	fd->out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd->out == -1)
		die(px, fd, "outfile error", 1);
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

char	**split_paths(char **envp)
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
