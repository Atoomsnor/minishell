/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:27 by roversch          #+#    #+#             */
/*   Updated: 2025/04/28 17:06:37 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../libft/libft.h"
#include "pipex.h"

#define HISTORY_SIZE 5

typedef enum e_type
{
	t_none,
	t_txt,
	t_flag,
	t_pipe,
	t_left,
	t_heredoc,
	t_right,
	t_append
}	t_type;

typedef enum e_error
{
	error_input_fail,
	error_read_file_fail,
	error_write_fail,
	error_
}	t_error;

typedef struct s_input
{
	struct s_input	*next;
	struct s_input	*prev;
	struct s_input	*head;
	char			*txt;
	t_type			type;
}	t_input;

typedef struct s_shell
{
	t_input	**curr_input;
	char	*in;
	char	**envp;
	char	*hist[HISTORY_SIZE];
	int		hist_count;
}	t_shell;

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

/* pipex_utils */
void	die(t_px *px, t_fd *fd, const char *msg, int exit_code);
void	build_structs(t_px *px, t_fd *fd, int argc, char **argv);
void	free_array(char **array);
char	**split_paths(char **envp);
char	*find_path(char **paths, char *cmd);

/* pipex */
void	child(t_px *px, t_fd *fd);
void	parent(t_px *px, t_fd *fd, int start);
void	here_child(t_px *px, t_fd *fd);
void	here_doc(t_px *px, t_fd *fd);
int		pipex(int argc, char **argv, char **envp, int append);

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
