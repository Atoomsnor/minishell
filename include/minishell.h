/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:27 by roversch          #+#    #+#             */
/*   Updated: 2025/05/22 15:19:26 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <signal.h>
#include "../libft/libft.h"
#include "struct.h"

extern volatile sig_atomic_t	g_signalreceived;

/* main */
int		sig_interrupted();
int		shelly(char **envp);
int		main(int argc, char **argv, char **envp);

/* history */
void	history(char *in); //maybe add into main?

/* signals */
void	sighandler(int signal);
void	sigint_handler(int signal);
void	init_signals(void);

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

/* lexer */
char	**ft_string_split(char const *s, char c);
t_type	find_type(char *in);
int		check_txt(t_input *input, int i);
t_input	**matrix_to_list(char **mat);
t_input	*parse_list(t_input *input);
t_input	**init_list(char *in);

/* murder */
void	shoot_error(t_error error);
void	lynch_exec(t_exec **exec);
void	shank_input(t_input **input);
void	die(t_exec **exec, t_input **input, t_error error);

/* parser */
void	free_array(char **array);
char	**split_paths(void);
char	*find_path(char **paths, char *cmd);
int		count_cmds(t_input *input);
int		count_till_pipe(t_input *input);
int		find_in(t_input *input);
int		find_out(t_input *input);
t_exec	*fill_exec(t_input **input);
int		is_buildin(char *cmd);
char	*cmd_to_path(t_exec *cmd);
t_exec **tokens_to_exec(t_input **input);

/* execute */
int		run_builtin(t_exec *exec, int fd, char **envp);
void	child(t_exec *exec, char **envp);
int		execute(t_exec **exec, char **envp);

/* builtins */
int		pwd(int fd);
int		echo(int fd, char **to_write);
int		cd(char *path);
t_var	*bi_export(char *var_name, char *str);
void	unset(char *name);
void	bi_exit();
void	env(char **envp);

/* pipe_parsing */
// int		mat_count(char **mat);
// void	move_list(char **input, int count);
// void	check_io(char **input, int *count);
// void	check_io_hd(char **input, int *count);
// void	print_matrix(char **input);
// void	pipe_parser(char *in, char **envp);

/* pipex */
// int		file_handler(t_shell *shell);
// int		file_h(t_shell *shell);

/* (ish) singlecmd */
// void	singleparent(t_px *px, t_fd *fd, int start);
// void	remove_path(t_px *px);
// void	singlecmd(char *cmd, char **envp);

// /* lists */
// void	ft_lstsethead(t_input **lst, t_input *head);
// int		ft_lstsize(t_input *lst);
// t_input	*ft_lstnew(void *content);
// t_input	*ft_lstlast(t_input *lst);
// void	ft_lstadd_back(t_input **lst, t_input *new_lst);
// void	ft_lstadd_front(t_input **lst, t_input *new_lst);
// void	ft_lstdelone(t_input *lst);

// void printlist(t_input *c);
// int	open_outfiles(t_input *in);

// t_cmd      *init_cmd(void);
// void       add_argument(t_cmd *cmd, char *arg);
// t_cmd      *parse_cmds(t_input *input);
// // void       execute_cmds(t_shell *shell);
// void       handle_error(const char *msg);

#endif
