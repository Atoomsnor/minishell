/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:27 by roversch          #+#    #+#             */
/*   Updated: 2025/04/29 16:57:11 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"
#include "struct.h"
#include "pipex.h"

/* main */
void	sighandler(int signal);
void	init_signals(void);
int		main(int argc, char **argv, char **envp);

/* linked_list */
int		ft_lstsize(t_input *lst);
t_input	*ft_lstnew(void *content);
t_input	*ft_lstlast(t_input *lst);
void	ft_lstadd_back(t_input **lst, t_input *new_lst);
void	ft_lstadd_front(t_input **lst, t_input *new_lst);

/* list_utils */
void	ft_lstsethead(t_input **lst, t_input *head);
void	ft_lstdelone(t_input *lst);
void	ft_lstadd_next(t_input **lst, t_input *new_lst);
t_input	*list_move(t_input *lst, int times);

/* parser */
t_type	find_type(char *in);
t_input	**mat_to_list(char **mat);
t_input	*parse_list(t_input *input);
t_input	**init_list(t_shell *shell);

/* pipe_parsing */
int		mat_count(char **mat);
void	move_list(char **input, int count);
void	check_io(char **input, int *count);
void	check_io_hd(char **input, int *count);
void	print_matrix(char **input);
void	pipe_parser(char *in, char **envp);

/* pipex */
int		file_handler(t_shell *shell);

/* (ish) singlecmd */
void	singleparent(t_px *px, t_fd *fd, int start);
void	remove_path(t_px *px);
void	singlecmd(char *cmd, char **envp);

/* string_split */
//static int	ft_count(char const *s, char c);
//static int	ft_len(char const *s, char c);
//static char	**ft_free(char **split_str);
//static char	**ft_bigsplit(char const *s, char c, char **n_str);
char	**ft_string_split(char const *s, char c);

// /* lists */
// void	ft_lstsethead(t_input **lst, t_input *head);
// int		ft_lstsize(t_input *lst);
// t_input	*ft_lstnew(void *content);
// t_input	*ft_lstlast(t_input *lst);
// void	ft_lstadd_back(t_input **lst, t_input *new_lst);
// void	ft_lstadd_front(t_input **lst, t_input *new_lst);
// void	ft_lstdelone(t_input *lst);

void printlist(t_input *c);

#endif
