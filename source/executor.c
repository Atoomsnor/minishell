/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:33:44 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/20 16:18:23 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int run_builtin(t_exec *exec, int fd)
{
	if (ft_strncmp(exec->full_cmd[0], "echo", 5) == 0)
		echo(fd, &(exec->full_cmd[1]));
	else if (ft_strncmp(exec->full_cmd[0], "pwd", 4) == 0)
		pwd(fd);
	else if (ft_strncmp(exec->full_cmd[0], "cd", 3) == 0)
		cd(exec->full_cmd[1]);
	else if (ft_strncmp(exec->full_cmd[0], "export", 7) == 0)
		bi_export(exec->full_cmd[0], exec->full_cmd[1]); //wrong needs to be fixed
	else if (ft_strncmp(exec->full_cmd[0], "unset", 6) == 0)
		unset(exec->full_cmd[1]);
	else if (ft_strncmp(exec->full_cmd[0], "exit", 5) == 0)
		bi_exit();
	else if (ft_strncmp(exec->full_cmd[0], "env", 4) == 0)
		return (1); //needs to be added
	return (0);
}

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
	if (exec->full_path[0] == '\0')
		run_builtin(exec, exec->out_fd);
	else
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
			close (pipe_fd[0]);
			if (!exec[i + 1] && exec[i]->out_fd == 1)
				pipe_fd[1] = 1;
			if (exec[i]->out_fd == 1)
				exec[i]->out_fd = pipe_fd[1];
			printf("%i outfd %i\n", prev_fd, exec[i]->out_fd);
			// if (prev_fd != -1 && prev_fd != exec[i]->in_fd)
			// 	exec[i]->in_fd = prev_fd;
			// prev_fd = exec[i]->out_fd;
			child(exec[i], envp);
		}
		i++;
	}
	while (i--)
		wait(NULL);
	return (1);
}