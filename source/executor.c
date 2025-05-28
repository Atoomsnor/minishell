/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:33:44 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/28 20:22:28 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

void	run_builtin(t_exec *exec, int fd, char ***envp)
{
	if (ft_strncmp(exec->full_cmd[0], "echo", 5) == 0)
		echo(fd, &(exec->full_cmd[1]));
	else if (ft_strncmp(exec->full_cmd[0], "pwd", 4) == 0)
		pwd(fd);
	else if (ft_strncmp(exec->full_cmd[0], "cd", 3) == 0)
		cd(exec->full_cmd[1]);
	else if (ft_strncmp(exec->full_cmd[0], "export", 7) == 0)
		*envp = exporting(exec->full_cmd[1], *envp);
	else if (ft_strncmp(exec->full_cmd[0], "unset", 6) == 0)
		unset(exec->full_cmd[1], *envp);
	else if (ft_strncmp(exec->full_cmd[0], "exit", 5) == 0)
		bi_exit(exec);
	else if (ft_strncmp(exec->full_cmd[0], "env", 4) == 0)
		env(*envp);
}

void	child(t_exec *exec, int prev_fd, int has_next, char **envp)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (has_next)
	{
		close(exec->pipe[0]);
		dup2(exec->pipe[1], STDOUT_FILENO);
		close(exec->pipe[1]);
	}
	if (exec->in_fd != STDIN_FILENO)
		dup2(exec->in_fd, STDIN_FILENO);
	if (exec->out_fd != STDOUT_FILENO)
		dup2(exec->out_fd, STDOUT_FILENO);
	if (exec->full_path && exec->full_path[0] != '\0')
		execve(exec->full_path, exec->full_cmd, envp);
	exit(1);
}

int	execute(t_exec **exec, char **envp)
{
	pid_t	pid;
	int		i;
	int		prev_fd;

	i = 0;
	prev_fd = -1;
	while (exec[i])
	{
		if (exec[i + 1] && pipe(exec[i]->pipe) == -1)
			return (0);
		pid = fork();
		if (pid == -1)
			return (0);
		if (pid == 0 && !exec[i + 1] && exec[i]->full_path[0] == '\0')
			run_builtin(exec[i], exec[i]->out_fd, &envp); //moved this from child as dup2 would override its outfd
		//might have conflict with builtin+non buildin as < infile cat | echo hello > out2 makes me need to ctrl-d twice 
		else if (pid == 0)
			child(exec[i], prev_fd, exec[i + 1] != NULL, envp);
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (exec[i + 1])
			{
				close(exec[i]->pipe[1]);
				prev_fd = exec[i]->pipe[0];
			}
		}
		i++;
	}
	while (i--)
		wait(NULL);
	return (1);
}
