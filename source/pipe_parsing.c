/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 21:26:14 by nhendrik          #+#    #+#             */
/*   Updated: 2025/04/15 12:59:52 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <string.h>

// count matrix (2D Char Array)
int mat_count(char **mat)
{
	int i;

	i = 0;
	while (mat[i])
		i++;
	return (i);
}

// move items in list 1 left when from starting point
void move_list(char **input, int count)
{
	int i;
	int len;

	i = 0;
	len = 0;
	while (i < count)
	{
		len = ft_strlen(input[i + 1]);
		ft_bzero(input[i], ft_strlen(input[i]));
		ft_memmove(input[i], input[i + 1], len);
		input[i][len] = '\0';
		i++;
	}
	if (input[i])
	{
		free(input[i]);
		input[i] = NULL;
	}
}

//parse through the pipe-based inputs & convert to pipexable
void	pipe_parser(char *in, char **envp)
{
	int count;
	char **input;
	int i;

	input = ft_string_split(in, ' ');
	count = mat_count(input);
	i = 0;
	if (input[3][0] != '|')
	{
		input[2] = ft_strjoin(input[2], " ");
		input[2] = ft_strjoin(input[2], input[3]);
		move_list(&input[3], count - 3);
		count--;
	}
	while (input[i])
	{
		if (input[i][0] == '|' && input[i + 2][0] != '|' && input[i + 2][0] != '>')
		{
			input[i + 1] = ft_strjoin(input[i + 1], " ");
			input[i + 1] = ft_strjoin(input[i + 1], input[i + 2]);
			move_list(&input[i + 2], count - i - 2);
			count--;
		}
		i++;
	}
	i = 0;
	while (input[i])
	{
		if (input[i][0] == '<' || input[i][0] == '|' || input[i][0] == '>')
		{
			move_list(&input[i], count - i);
			count--;
		}
		i++;
	}
	i = 0;
	pipex(count, input, envp);
}