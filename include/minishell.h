/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:27 by roversch          #+#    #+#             */
/*   Updated: 2025/04/22 12:30:34 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "pipex.h"

/* main */
void		sighandler(int signal);
void		init_signals(void);
int			main(int argc, char **argv, char **envp);

/* pipe_parsing */
int			mat_count(char **mat);
void		move_list(char **input, int count);
void		check_io(char **input, int *count);
void		check_io_hd(char **input, int *count);
void		print_matrix(char **input);
void		pipe_parser(char *in, char **envp);

/* (done) pipex_utils */
void		die(t_px *px, t_fd *fd, const char *msg, int exit_code);
void		build_structs(t_px *px, t_fd *fd, int argc, char **argv);
void		free_array(char **array);
char		**split_paths(char **envp);
char		*find_path(char **paths, char *cmd);

/* (done) pipex */
void		child(t_px *px, t_fd *fd);
void		parent(t_px *px, t_fd *fd, int start);
void		here_child(t_px *px, t_fd *fd);
void		here_doc(t_px *px, t_fd *fd);
int			pipex(int argc, char **argv, char **envp, int append);

/* (ish) singlecmd */
void		singleparent(t_px *px, t_fd *fd, int start);
void		remove_path(t_px *px);
void		singlecmd(char *cmd, char **envp);

/* string_split */
//static int	ft_count(char const *s, char c);
//static int	ft_len(char const *s, char c);
//static char	**ft_free(char **split_str);
//static char	**ft_bigsplit(char const *s, char c, char **n_str);
char		**ft_string_split(char const *s, char c);
