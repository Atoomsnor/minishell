// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   pipex_utils.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/03/24 11:38:04 by roversch          #+#    #+#             */
// /*   Updated: 2025/05/20 17:48:58 by nhendrik         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../include/minishell.h"
// #include <stdlib.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <signal.h>

// void	z_die(t_px *px, t_fd *fd, const char *msg, int exit_code)
// {
// 	if (fd)
// 	{
// 		close(fd->in);
// 		close(fd->out);
// 		close(fd->pipe[0]);
// 		close(fd->pipe[1]);
// 	}
// 	free_array(px->paths);
// 	perror(msg);
// 	exit(exit_code);
// }

// void	build_structs(t_px *px, t_fd *fd, int argc, t_input **input)
// {
// 	px->argc = argc;
// 	px->input = input;
// 	px->paths = split_paths();
// 	if (!px->paths)
// 		z_die(px, NULL, "path error", 1);
// 	if (has_type(*input, t_heredoc, 0))
// 		fd->in = open("./minishell", O_RDONLY);
// 	else
// 		fd->in = open(input[0]->txt, O_RDONLY);
// 	if (fd->in == -1)
// 		z_die(px, fd, "infile error", 1);
// 	fd->out = 0;
// 	if (fd->out == -1)
// 		z_die(px, fd, "outfile error", 1);
// }