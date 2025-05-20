// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   ppxrevised.c                                       :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/05/03 00:55:27 by nhendrik          #+#    #+#             */
// /*   Updated: 2025/05/08 17:40:05 by roversch         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../include/minishell.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <readline/readline.h>
// #include <errno.h>
// #include <fcntl.h>

// void	build_str(t_px *px, t_fd *fd, int argc, t_input **input)
// {
// 	px->argc = argc;
// 	px->input = input;
// 	px->paths = split_paths(px->envp);
// 	if (!px->paths)
// 		die(px, NULL, "path error", 1);
// 	if (!has_type(*input, t_left, 0))
// 		fd->in = open("./minishell", O_RDONLY);
// 	else
// 		fd->in = open((*input)->next->txt, O_RDONLY);
// 	if (fd->in == -1)
// 		die(px, fd, "infile error", 1); 
// 	fd->out = 0;
// 	if (fd->out == -1)
// 		die(px, fd, "outfile error", 1);
// }

// char **i_to_c(t_px *px)
// {
// 	t_input *curr;
// 	char	**ret;

// 	curr = list_move(*px->input, px->i);
// 	if (curr->next->type == t_append || curr->next->type == t_right || curr->next->type == t_pipe)
// 		ret = ft_calloc(2, sizeof(char *));
// 	else
// 	 	ret = ft_calloc(3, sizeof(char *));
// 	ret[0] = curr->txt;
// 	if (curr->next->type == t_flag)
// 		ret[1] = curr->next->txt;
// 	else if (curr->next->type == t_txt)
// 	{
// 		curr = curr->next;
// 		while (curr && curr->type == t_txt)
// 		{
// 			if (ret[1])
// 				ret[1] = ft_strjoin (ret[1], " ");
// 			ret[1] = ft_strjoin(ret[1], curr->txt);
// 			curr = curr->next;
// 		}
// 	}
// 	return (ret);
// }

// char **inptocmd(t_px *px)
// {
// 	int		count;
// 	t_input *curr;
// 	char	**ret;

// 	count = 0;
// 	curr = list_move(*px->input, px->i);
// 	while (curr && (curr->type == t_txt || curr->type == t_flag))
// 	{
// 		count++;
// 		curr = curr->next;
// 	}
// 	ret = ft_calloc(count + 1, sizeof(char *));
// 	curr = list_move(*px->input, px->i);
// 	count = 0;
// 	while (curr && (curr->type == t_txt || curr->type == t_flag))
// 	{
// 		ret[count] = curr->txt;
// 		curr = curr->next;
// 		count++;
// 	}
// 	print_matrix(ret);
// 	return (ret);
// }

// void	scold(t_px *px, t_fd *fd)
// {
// 	char	*full_path;

// 	px->cmd = i_to_c(px);
// 	print_matrix(px->cmd);
// 	if (!px->cmd)
// 		die(px, fd, "malloc error", 1);
// 	full_path = find_path(px->paths, px->cmd[0]);
// 	if (!full_path)
// 	{
// 		free_array(px->cmd);
// 		die(px, fd, "path error", 127);
// 	}
// 	if (fd->in != STDIN_FILENO)
// 	{
// 		dup2(fd->in, STDIN_FILENO);
// 		close(fd->in);
// 	}
// 	if (fd->out != STDOUT_FILENO)
// 	{
// 		dup2(fd->out, STDOUT_FILENO);
// 		close(fd->out);
// 	}
// 	execve(full_path, px->cmd, px->envp);
// 	free(full_path);
// 	free_array(px->cmd);
// 	die(px, fd, "execve error", 126);
// }

// int	increment_to_next(t_px *px)
// {
// 	int i;
// 	t_input *in;

// 	in = list_move(*px->input, px->i);
// 	i = 0;
// 	while (in && in->type != t_append && in->type != t_right && in->type != t_pipe)
// 	{
// 		in = in->next;
// 		i++;
// 	}
// 	printf("%i\n", i);
// 	return (i);
// }

// void	parrot(t_px *px, t_fd *fd, int start)
// {
// 	pid_t	pid;

// 	px->i = start;
// 	while (px->i < px->argc - 1)
// 	{
// 		if (pipe(fd->pipe) == -1)
// 			die(px, fd, "pipe error", 1);
// 		pid = fork();
// 		if (pid == -1)
// 			die(px, fd, "fork error", 1);
// 		if (pid == 0)
// 		{
// 			close(fd->pipe[0]);
// 			if (px->i < px->argc - 2)
// 				fd->out = fd->pipe[1];
// 			scold(px, fd);
// 		}
// 		close(fd->pipe[1]);
// 		if (px->i != 2)
// 			close(fd->in);
// 		fd->in = fd->pipe[0];
// 		waitpid(pid, NULL, 0); //we should wait for child outside the loop.
// 		//check that sleep 3 | sleep 3 does 3 and not 6.
// 		px->i += increment_to_next(px) + 1;
// 	}
// }

// void	here_c(t_px *px, t_fd *fd)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 			exit(EXIT_SUCCESS);
// 		if (ft_strncmp(line, px->input[1]->txt, ft_strlen(px->input[1]->txt)) == 0)
// 		{
// 			free(line);
// 			exit(EXIT_SUCCESS);
// 		}
// 		ft_putendl_fd(line, fd->pipe[1]);
// 		free(line);
// 	}
// }

// void	here_d(t_px *px, t_fd *fd)
// {
// 	int		hid;

// 	if (pipe(fd->pipe) == -1)
// 		die(px, fd, "pipe error", 1);
// 	hid = fork();
// 	if (hid == -1)
// 		die(px, fd, "fork error", 1);
// 	if (hid == 0)
// 	{
// 		close(fd->pipe[0]);
// 		here_c(px, fd);
// 	}
// 	close(fd->pipe[1]);
// 	fd->in = fd->pipe[0];
// 	waitpid(hid, NULL, 0);
// 	parent(px, fd, 2);
// }

// int	file_h(t_shell *shell)
// {
// 	t_px		px;
// 	t_fd		fd;
// 	int			argc;

// 	argc = ft_lstsize(*shell->curr_input);
// 	printf("argc: %i\n", argc);
// 	if (argc < 2)
// 		return (perror("input error"), 1);
// 	px.envp = shell->envp;
// 	build_str(&px, &fd, argc, shell->curr_input);
// 	if (argc < 3)
// 		singleparent(&px, &fd, 2);
// 	else if (has_type(*shell->curr_input, t_heredoc, 0))
// 	{
// 		if (argc < 3)
// 		{
// 			free_array(px.paths);
// 			return (perror("input error"), 1);
// 		}
// 		here_d(&px, &fd);
// 	}
// 	else
// 		parrot(&px, &fd, 2);
// 	free_array(px.paths);
// 	return (0);
// }