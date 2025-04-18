/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:38:15 by roversch          #+#    #+#             */
/*   Updated: 2025/04/17 12:45:53 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <errno.h>

void	child(t_px *px, t_fd *fd)
{
	char	*full_path;

	px->cmd = ft_split(px->argv[px->i], ' ');
	if (!px->cmd)
		die(px, fd, "malloc error", 1);
	full_path = find_path(px->paths, px->cmd[0]);
	if (!full_path)
	{
		free_array(px->cmd);
		die(px, fd, "path error", 127);
	}
	dup2(fd->in, STDIN_FILENO);
	close(fd->in);
	dup2(fd->out, STDOUT_FILENO);
	close(fd->out);
	execve(full_path, px->cmd, px->envp);
	free(full_path);
	free_array(px->cmd);
	die(px, fd, "execve error", 126);
}

void	parent(t_px *px, t_fd *fd, int start)
{
	pid_t	pid;

	px->i = start;
	while (px->i < px->argc - 1)
	{
		if (pipe(fd->pipe) == -1)
			die(px, fd, "pipe error", 1);
		pid = fork();
		if (pid == -1)
			die(px, fd, "fork error", 1);
		if (pid == 0)
		{
			close(fd->pipe[0]);
			if (px->i < px->argc - 2)
				fd->out = fd->pipe[1];
			child(px, fd);
		}
		close(fd->pipe[1]);
		if (px->i != 2)
			close(fd->in);
		fd->in = fd->pipe[0];
		waitpid(pid, NULL, 0);
		px->i++;
	}
}

void	here_child(t_px *px, t_fd *fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			exit(EXIT_SUCCESS);
		if (ft_strncmp(line, px->argv[1], ft_strlen(px->argv[1])) == 0)
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		ft_putendl_fd(line, fd->pipe[1]);
		free(line);
	}
}

void	here_doc(t_px *px, t_fd *fd)
{
	int		hid;

	if (pipe(fd->pipe) == -1)
		die(px, fd, "pipe error", 1);
	hid = fork();
	if (hid == -1)
		die(px, fd, "fork error", 1);
	if (hid == 0)
	{
		close(fd->pipe[0]);
		here_child(px, fd);
	}
	close(fd->pipe[1]);
	fd->in = fd->pipe[0];
	waitpid(hid, NULL, 0);
	parent(px, fd, 2);
}

int	pipex(int argc, char **argv, char **envp)
{
	t_px		px;
	t_fd		fd;

	if (argc < 4)
		return (perror("input error"), 1);
	px.envp = envp;
	build_structs(&px, &fd, argc, argv);
	if (ft_strncmp(argv[0], "here_doc", 9) == 0)
	{
		if (argc < 5)
		{
			free_array(px.paths);
			return (perror("input error"), 1);
		}
		here_doc(&px, &fd);
	}
	else
		parent(&px, &fd, 1);
	free_array(px.paths);
	return (0);
}

//int main(int argc, char **argv, char **envp)
//{
//	return (pipex(argc, argv, envp));
//}