/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:33:44 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/19 17:48:02 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void child(t_exec *exec, char **envp)
{
	if (exec->in_fd != STDIN_FILENO)
	{
		dup2(exec->in_fd, STDIN_FILENO);
		close(exec->in_fd);
	}
	if (exec->out_fd != STDOUT_FILENO)
	{
		dup2(exec->out_fd, STDOUT_FILENO);
		close(exec->out_fd);
	}
	execve(exec->full_path, exec->full_cmd, envp);
	//error
}

int execute(t_exec **exec, char **envp)
{
	int		i;
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	pid;

	i = 0;
	prev_fd = -1;
	while (exec[i])
	{
		if (pipe(pipe_fd) == -1)
			return (0); // error
		pid = fork();
		if (pid == -1)
			return (0); // error
		if (pid == 0)
		{
			// close (pipe_fd[0]);
			// if (exec[i]->out_fd == 1)
			// 	exec[i]->out_fd = pipe_fd[1];
			child(exec[i], envp);
		}
		i++;
	}
	while (i--)
		wait(NULL);
	return (1);
}