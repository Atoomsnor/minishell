/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:33:44 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/26 13:06:17 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int	run_builtin(t_exec *exec, int fd, char **envp)
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
		env(envp);
	return (0);
}

void	child(t_exec *exec, char **envp)
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
		run_builtin(exec, exec->out_fd, envp);
	else
		execve(exec->full_path, exec->full_cmd, envp);
	exit (1);
}

int	execute(t_exec **exec, char **envp)
{
	pid_t	pid;
	int		i;
	int		pipe_fd[2];
	int		prev_fd;

	i = 0;
	prev_fd = -1;
	while (exec[i])
	{
		if (pipe(pipe_fd) == -1)
			return (0);
		pid = fork();
		if (pid == -1)
			return (0);
		if (exec[i]->out_fd == 1 && exec[i + 1])
			exec[i]->out_fd = pipe_fd[1];
		if (prev_fd != -1 && exec[i]->in_fd == 0)
			exec[i]->in_fd = prev_fd;
		// printf("infd %i outfd %i prevfd %i\nfullpath %s\n", exec[i]->in_fd, exec[i]->out_fd, prev_fd, exec[i]->full_path);
		if (pid == 0 && !exec[i + 1] && exec[i]->full_path[0] == '\0')
			run_builtin(exec[i], exec[i]->out_fd, envp);
		else if (pid == 0)
			child(exec[i], envp);
		prev_fd = exec[i]->out_fd;
		i++;
	}
	while (i--)
		wait(NULL);
	return (1);
}
