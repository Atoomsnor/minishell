/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 21:26:14 by nhendrik          #+#    #+#             */
/*   Updated: 2025/04/21 11:44:48 by nhendrik         ###   ########.fr       */
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

// checks for multipe input- & output-files for regular piping
void check_io(char **input, int *count)
{
	int i;

	i = 1;
	while (input[i])
	{
		if (input[i][0] == '<')
		{
			while (i-- > 0)
			{
				move_list(&input[i], *count - i);
				(*count)--;
			}	
			i = 0;
		}
		else if (input[i][0] == '>' && i != *count - 2)
		{
			move_list(&input[i], *count - i);
			(*count)--;
			move_list(&input[i], *count - i);
			(*count)--;
			i = 0;
		}
		i++;
	}
}

// checks for multiple input- & output-files for heredocing
void check_io_hd(char **input, int *count)
{
	int i;

	i = 1;
	while (input[i])
	{
		if (!(ft_strncmp(input[i], "<<", 3)) || input[i][0] == '<')
		{
			while (i-- > 0)
			{
				move_list(&input[i], *count - i);
				(*count)--;
			}
			i = 0;
		}
		else if ((!(ft_strncmp(input[i], ">>", 3)) || input[i][0] == '>') && i != *count - 2)
		{
			move_list(&input[i], *count - i);
			(*count)--;
			move_list(&input[i], *count - i);
			(*count)--;
			i = 0;
		}
		i++;
	}
}

void print_matrix(char **input)
{
	for (int i = 0; input[i]; i++)
		ft_printf("%s\n", input[i]);
	ft_printf("endl\n");
}

// parse through the pipe-based inputs & convert to pipexable
void	pipe_parser(char *in, char **envp)
{
	int count;
	char **input;
	int i;

	ft_printf("%s\n", in);
	input = ft_string_split(in, ' ');
	print_matrix(input);
	count = mat_count(input);
	check_io_hd(input, &count);
	if (ft_strncmp(input[0], "<<", 3) == 0)
		input[0] = "here_doc";
	if (ft_strncmp(input[count - 1], "<<", 3) == 0)
		input[count - 1]  = "append";
	i = 0;
	if (input[3][0] != '|' && input[3][0] != '>')
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
	print_matrix(input);
	pipex(count, input, envp);
}