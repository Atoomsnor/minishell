/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singlecmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:36:30 by roversch          #+#    #+#             */
/*   Updated: 2025/04/22 12:28:11 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void	singleparent(t_px *px, t_fd *fd, int start)
{
	pid_t	pid;

	px->i = start;
	pid = fork();
	if (pid == -1)
		die(px, fd, "fork error", 1);
	if (pid == 0)
		child(px, fd);
	else
	{
		close(fd->in);
		close(fd->out);
		waitpid(pid, NULL, 0);
	}
}

void	remove_path(t_px *px)
{
	char	*last;

	last = ft_strrchr(px->argv[0], '/');
	if (!last)
		return ;
	px->argv[0] = last + 1;
}

void	singlecmd(char *cmd, char **envp)
{
	t_px	px;
	t_fd	fd;
	pid_t	sid;

	px.envp = envp;
	px.argc = 1;
	px.i = 0;
	px.argv = &cmd;
	fd.in = STDIN_FILENO;
	fd.out = STDOUT_FILENO;
	if (cmd[0] == '/')
		remove_path(&px);
	px.paths = split_paths(envp);
	if (!px.paths)
		die(&px, NULL, "path error", 1);
	sid = fork();
	if (sid == -1)
		die(&px, &fd, "fork error", 1);
	if (sid == 0)
		child(&px, &fd);
	else
		waitpid(sid, NULL, 0);
	free_array(px.paths);
}
