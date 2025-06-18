/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:27 by roversch          #+#    #+#             */
/*   Updated: 2025/06/18 16:33:19 by nhendrik         ###   ########.fr       */
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
int		shelly(char ***envp, int retval, char **hist);
int		main(int argc, char **argv, char **envp);

/* history */
void	history(char *in, char **hist);
void	add_heredoc_hist(int fd, char **hist);

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
void	lynch_exec(t_exec **exec);
void	shank_input(t_input **input);
void	die(t_exec **exec, t_input **input, char *error);

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
t_exec **tokens_to_exec(t_input **input, char **envp, int retval, char **hist);

/* execute */
void	run_builtin(t_exec *exec, int fd, char ***envp, int child);
void	child(t_exec *exec, int prev_fd, int has_next, char **envp);
int		execute(t_exec **exec, char **envp);

/* builtins */
int		pwd(int fd);
int		echo(int fd, char **to_write);
int		cd(char *path);
char	**exporting(char *str, char **env);
void	unset(char *name, char **env);
void	bi_exit(t_exec *exec, int child);
void	env(char **envp, int fd);

/* quote */
t_input	**dequote(char **env, int retval, t_input **input);
// int		print_strings_fd(int fd, ...);
int		has_char(char *str, char c);

/* wildcard */
char	*handle_wildcard(char *str, char **env, int retval);

/* heredoc */
int		run_here_doc(t_input **input, char *delimiter, char **hist);
void	printlist(t_input *c, int i);

#endif
