/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:33:44 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/03 14:09:44 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int	run_builtin(t_exec *exec, int fd, char ***envp, int child)
{
	if (ft_strncmp(exec->full_cmd[0], "echo", 5) == 0)
		echo(fd, &(exec->full_cmd[1]));
	else if (ft_strncmp(exec->full_cmd[0], "pwd", 4) == 0)
		pwd(fd);
	else if (ft_strncmp(exec->full_cmd[0], "cd", 3) == 0)
	{
		if (!cd(exec->full_cmd, envp))
			return (1);
	}
	else if (ft_strncmp(exec->full_cmd[0], "export", 7) == 0)
	{
		if (!exec->full_cmd[1])
			env(*envp, fd);
		else if (!exporting(exec->full_cmd[1], envp))
			return (ft_putstr_fd(" not a valid identifier\n", 2), 1);
	}
	else if (ft_strncmp(exec->full_cmd[0], "unset", 6) == 0)
		unset(exec->full_cmd[1], envp);
	else if (ft_strncmp(exec->full_cmd[0], "exit", 5) == 0)
		bi_exit(exec, child);
	else if (ft_strncmp(exec->full_cmd[0], "env", 4) == 0)
		env(*envp, fd);
	if (child)
		exit(0);
	return (0);
}

void	child(t_exec *exec, int prev_fd, int has_next, char **envp)
{
	int	ret;

	ret = 1;
	if (exec->err_msg && exec->in_fd == 0 && !ft_strncmp(exec->full_cmd[0], "cat", 4))
		exit(0);
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
		ret = execve(exec->full_path, exec->full_cmd, envp);
	else
		run_builtin(exec, exec->out_fd, &envp, 1);
	exit(ret);
}

void	set_fds(t_exec *exec, int *prev_fd, t_exec *next)
{
	if ((*prev_fd) != -1)
		close((*prev_fd));
	if (next)
	{
		close(exec->pipe[1]);
		(*prev_fd) = exec->pipe[0];
	}
}

void check_pipe(int *pipe, int prev_fd)
{
	char buff[2];

	if (prev_fd == -1)
		return ;
	if (read(pipe[0], buff, 1) < 1)
	{
		write(2, " Broken pipe\n", 13);
	}
	else if (buff[0] == '\0')
	{
		write(2, " Broken pipe\n", 13);
	}
}

int	execute(t_exec **exec, char **envp)
{
	pid_t	pid;
	int		i;
	int		status;
	int		prev_fd;

	i = 0;
	prev_fd = -1;
	while (exec[i])
	{
		if ((exec[i + 1] || exec[i]->err_msg) && pipe(exec[i]->pipe) == -1)
			return (0);
		pid = fork();
		if (pid == -1)
			return (0);
		signal(SIGINT, SIG_IGN);
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			child(exec[i], prev_fd, exec[i + 1] != NULL, envp);
		}
		else
			set_fds(exec[i], &prev_fd, exec[i + 1]);
		i++;
	}
	while (i--)
	{
		wait(&status);
		if (status == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		if (status >= 256)
			return (status >> 8);
	}
	signal(SIGINT, sigint_handler);
	return (0);
}
