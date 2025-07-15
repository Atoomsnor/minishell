/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:27 by roversch          #+#    #+#             */
/*   Updated: 2025/07/15 18:02:38 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "struct.h"
# include <signal.h>

extern volatile sig_atomic_t	g_signalreceived;

/* and_i_quote */
char			*trim_quotes(char *str, char quote, int start);
int				has_char(char *str, char c);
char			find_first_quote(char *str);
t_input			**dequote(char **env, int retval, t_input **input);

/* builtin */
int				pwd(int fd);
int				echo(int fd, char **to_write);
int				bi_exit(t_exec **exec, char ***envp, t_history *hist);
int				cd(char **path, char ***env);
int				exports(char **str, char ***env);
void			unset(char *name, char ***env);
void			env(char **envp, int fd);

/* executor */
int				run_builtin(t_exec *exec, int fd, char ***envp, int child);
void			child(t_exec *exec, int prev_fd, int has_next, char **envp);
int				execute(t_exec **exec, char **envp);

/* find_fds */
int				find_in_out(t_input *input, int *in_fd,
					int *out_fd, char **error_msg);
int				file_is_empty(char *path);

/* free_utils */
void			*free_array(char **array);
char			*ft_strjoin_free(char *s1, char *s2, int liberate);
char			*ft_substr_free(char *s, unsigned int start, size_t len);
void			*free_and_null(void *ptr);
void			*malloc_error_free(void *f);

/* here_doc */
int				check_heredoc(t_input *input, int *retval, char **env);

/* history */
void			history(t_history *hist);
t_history		*init_hist(void);

/* lex_list */
t_input			**matrix_to_list(char **matrix);
int				rotation(int *i, t_input *cpy);
t_input			*parse_list(t_input *input);

/* lexer */
char			find_first_quote_len(char *str);
int				check_txt(t_input *input, int i);
t_input			**init_list(char *in, int skip);

/* linked list*/
t_input			*ft_lstnew(void *content, int i);
t_input			*ft_lstlast(t_input *lst);
int				ft_lstadd_back(t_input **lst, t_input *new_lst);

/* list utils */
void			ft_lstsethead(t_input **lst, t_input *head);
void			ft_lstdelone(t_input *lst);
void			ft_lstadd_next(t_input **lst, t_input *new_lst);
void			ft_lstcopy(t_input *input, t_input *next);

/* murder */
void			lynch_exec(t_exec **exec);
void			*shank_input(t_input **input);
void			*burn_history(t_history *hist);
void			*die(t_exec **exec, t_input **input, char *error, void *ret);

/* parser_checks */
int				check_dir(char *str, char **error_msg, int i);
int				is_buildin(char *cmd);
t_input			**check_empty_txt(t_input **input);
int				check_access(char *path, char **error_msg);

/* parser_error */
void			*adjust_error(char **error_msg, char *err1, char *err2);
void			*set_retval(int *retval, int val);

/* parser_utils */
void			rotate_input(t_input **input);
int				rotate_past_pipe(t_input **input, int count);
int				has_type(t_input *input, t_type type);
int				count_cmds(t_input *input);
int				count_till_pipe(t_input *input);

/* parser */
t_exec			**tokens_to_exec(t_input **input, char **envp, int *retval);

/* pathfinding */
char			*cmd_to_path(t_exec *cmd, char **error_msg, char **envp);

/* shell */
int				shelly(char ***envp, int retval, t_history *hist);

/* signal */
void			sighandler(int signal);
void			sigint_handler(int signal);
void			init_signals(void);

/* string_split */
char			**ft_string_split(char const *s, char c);

/* utils */
size_t			ft_strmcpy(char **dest, const char *src);
char			**ft_matdup(char **mat);
unsigned long	ft_atoul(const char *nptr);

/* wildcard */
char			*handle_wildcard(char *str, char **env, int retval, int recur);
char			*set_var_name(char *str, char **env, int retval, int *i);

#endif
