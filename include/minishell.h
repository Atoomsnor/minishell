/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:27 by roversch          #+#    #+#             */
/*   Updated: 2025/04/21 10:54:22 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "pipex.h"

char	**ft_string_split(char const *s, char c);
void	pipe_parser(char *in, char **envp);

void check_io(char **input, int *count);
void check_io_hd(char **input, int *count);

// void	singlecmd(int argc, char **argv, char **envp);