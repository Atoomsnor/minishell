/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singlecmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:36:30 by roversch          #+#    #+#             */
/*   Updated: 2025/04/21 11:19:04 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void	build_struct(t_px *px, t_fd *fd, int argc, char **argv)
{
	px->argc = argc;
	px->argv = argv;
	px->paths = split_paths(px->envp);
	if (!px->paths)
		die(px, NULL, "path error", 1);
	fd->in = open("./minishell", O_RDONLY);
	if (fd->in == -1)
		die(px, fd, "infile error", 1);
	fd->out = 0;
	if (fd->out == -1)
		die(px, fd, "outfile error", 1);
}

void	singlecmd(char *cmd, char **envp)
{
	t_px		px;
	t_fd		fd;
	pid_t		pid;

	px.envp = envp;
	// px.i = 2;
	build_structs(&px, &fd, 1, &cmd);
	pid = fork();
	if (pid == -1)
		die(&px, &fd, "fork error", 1);
	if (pid == 0)
	{
		close(fd.pipe[0]);
		if (px.i < px.argc - 2)
			fd.out = fd.pipe[1];
		singleparent(&px, &fd, 1);
	}
}