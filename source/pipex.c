/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:38:15 by roversch          #+#    #+#             */
/*   Updated: 2025/04/29 16:28:27 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <errno.h>

void	child(t_px *px, t_fd *fd)
{
	char	*full_path;

	px->cmd = ft_split(px->input[px->i]->txt, ' ');
	if (!px->cmd)
		die(px, fd, "malloc error", 1);
	full_path = find_path(px->paths, px->cmd[0]);
	if (!full_path)
	{
		free_array(px->cmd);
		die(px, fd, "path error", 127);
	}
	if (fd->in != STDIN_FILENO)
	{
		dup2(fd->in, STDIN_FILENO);
		close(fd->in);
	}
	if (fd->out != STDOUT_FILENO)
	{
		dup2(fd->out, STDOUT_FILENO);
		close(fd->out);
	}
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
		waitpid(pid, NULL, 0); //we should wait for child outside the loop.
		//check that sleep 3 | sleep 3 does 3 and not 6.
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
		if (ft_strncmp(line, px->input[1]->txt, ft_strlen(px->input[1]->txt)) == 0)
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

int	file_handler(t_shell *shell)
{
	t_px		px;
	t_fd		fd;
	int			argc;

	argc = ft_lstsize(shell->curr_input[0]) - 1;
	if (argc < 2)
		return (perror("input error"), 1);
	px.envp = shell->envp;
	build_structs(&px, &fd, argc, shell->curr_input);
	if (argc < 3)
		singleparent(&px, &fd, 1);
	else if (has_type(shell->curr_input[0], t_heredoc))
	{
		if (argc < 3)
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
