/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singlecmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:36:30 by roversch          #+#    #+#             */
/*   Updated: 2025/04/17 16:20:20 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	singlecmd(int argc, char **argv, char **envp)
{
	t_px		px;
	t_fd		fd;
	char	*full_path;

	px.envp = envp;
	px.i = 2;
	build_structs(&px, &fd, argc, argv);
	px.cmd = ft_split(px.argv[px.i], ' ');
	if (!px.cmd)
		die(&px, &fd, "malloc error", 1);
	full_path = find_path(px.paths, px.cmd[0]);
	if (!full_path)
	{
		free_array(px.cmd);
		die(&px, &fd, "path error", 127);
	}
	dup2(fd.in, STDIN_FILENO);
	close(fd.in);
	dup2(fd.out, STDOUT_FILENO);
	close(fd.out);
	execve(full_path, px.cmd, px.envp);
	free(full_path);
	free_array(px.cmd);
	die(&px, &fd, "execve error", 126);
}