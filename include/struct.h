/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:36:55 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/17 13:43:09 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

#include "../libft/libft.h"

#define HISTORY_SIZE 500

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

// typedef struct s_prompt
// {
// 	t_list	*cmds;
// 	char	**envp;
// 	pid_t	pid;	
// }	t_prompt;

typedef struct s_exec
{
	char	**full_cmd;
	char	*full_path;
	int		pipe[2];
	int		in_fd;
	int		out_fd;
}	t_exec;

// typedef enum e_error
// {
// 	error_input_fail,
// 	error_read_file_fail,
// 	error_write_fail,
// 	error_fill_exec,
// 	error_cmd_to_path,
// 	error_
// }	t_error;

typedef struct s_input
{
	struct s_input	*next;
	struct s_input	*prev;
	struct s_input	*head;
	char			*txt;
	t_type			type;
	int				hd_fd;
}	t_input;

typedef struct s_var
{
	char *name;
	char *content;
} t_var;

/* struct utils*/
int has_type(t_input *input, ...);

#endif
