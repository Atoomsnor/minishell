/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:36:55 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/08 17:38:19 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# define HISTORY_SIZE 500

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

typedef struct s_exec
{
	char	**full_cmd;
	char	*full_path;
	char	*err_msg;
	int		pipe[2];
	int		in_fd;
	int		out_fd;
}	t_exec;

typedef struct s_history
{
	char	*hist[HISTORY_SIZE];
	char	*in;
	int		hist_count;
}	t_history;

typedef struct s_input
{
	struct s_input	*next;
	struct s_input	*prev;
	struct s_input	*head;
	char			*txt;
	t_type			type;
	int				hd_fd;
}	t_input;

#endif
