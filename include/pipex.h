/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:28:58 by roversch          #+#    #+#             */
/*   Updated: 2025/04/21 14:08:16 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"

typedef struct s_fd
{
	int	in;
	int	out;
	int	pipe[2];
}	t_fd;

typedef struct s_px
{
	int		i;
	int		append;
	int		argc;
	char	**argv;
	char	**paths;
	char	**envp;
	char	**cmd;
}	t_px;

/* Error Handling */
void	die(t_px *px, t_fd *fd, const char *msg, int exit_code);

/* Process Management */
void	here_doc(t_px *px, t_fd *fd);
void	here_child(t_px *px, t_fd *fd);
void	parent(t_px *px, t_fd *fd, int start);
void	child(t_px *px, t_fd *fd);
void	singleparent(t_px *px, t_fd *fd, int start);

/* Path Handling */
char	**split_paths(char **envp);
char	*find_path(char **paths, char *cmd);
void	free_array(char **array);

/* Main Function */
void	build_structs(t_px *px, t_fd *fd, int argc, char **argv);
int		pipex(int argc, char **argv, char **envp, int append);
void	print_matrix(char **input);

#endif