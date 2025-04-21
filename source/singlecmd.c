/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singlecmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:36:30 by roversch          #+#    #+#             */
/*   Updated: 2025/04/21 10:51:28 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../include/minishell.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>

// void	singlecmd(t_px *px, t_fd *fd, int start)
// {
// 	t_px		px;
// 	t_fd		fd;
// 	pid_t		pid;

// 	px.envp = envp;
// 	// px.i = 2;
// 	build_structs(&px, &fd, argc, argv);
// 	pid = fork();
// 	if (pid == -1)
// 		die(px, fd, "fork error", 1);
// 	if (pid == 0)
// 	{
// 		close(fd->pipe[0]);
// 		if (px->i < px->argc - 2)
// 			fd->out = fd->pipe[1];
// 		child(px, fd);
// 	}
// }