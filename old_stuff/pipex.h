/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:28:58 by roversch          #+#    #+#             */
/*   Updated: 2025/06/09 13:12:58 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
#include "struct.h"

typedef struct s_cmd // needs to be goneso
{
	struct s_cmd	*next;
	char			**args;
	int				in_fd;
	int				out_fd;
	int				pipe[2];
}	t_cmd;

typedef struct s_fd // needs to be goneso
{
	int	in;
	int	out;
	int	pipe[2];
}	t_fd;

typedef struct s_px //goneso
{
	int		i;
	int		append;
	int		argc;
	char	**paths;
	char	**envp;
	char	**cmd;
	t_input	**input;
}	t_px;

/* Error Handling */
void	z_die(t_px *px, t_fd *fd, const char *msg, int exit_code);

/* Process Management */
void	here_doc(t_px *px, t_fd *fd);
void	here_child(t_px *px, t_fd *fd);
void	parent(t_px *px, t_fd *fd, int start);
void	z_child(t_px *px, t_fd *fd);
void	singleparent(t_px *px, t_fd *fd, int start);

/* Path Handling */
char	**split_paths(void);
char	*find_path(char **paths, char *cmd);
// void	free_array(char **array);

/* Main Function */
void	build_structs(t_px *px, t_fd *fd, int argc, t_input **input);
// int		file_handler(t_shell *shell);
void	print_matrix(char **input);

#endif