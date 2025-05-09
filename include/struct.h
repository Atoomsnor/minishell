/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:36:55 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/08 17:49:58 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

#include "../libft/libft.h"

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

typedef struct s_cmd
{
	struct s_cmd	*next;
	char			**args;
	int				in_fd;
	int				out_fd;
	int				pipe[2];
}	t_cmd;


typedef struct s_fd
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

typedef struct s_var
{
	char *name;
	char *content;
} t_var;

/* struct utils*/
int has_type(t_input *input, ...);

#endif
