/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 21:26:14 by nhendrik          #+#    #+#             */
/*   Updated: 2025/04/14 21:26:14 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

int mat_count(char **mat)
{
	int i;

	i = 0;
	while (mat[i])
		i++;
	return (i);
}

void move_list(char **input, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		ft_bzero(input[i], ft_strlen(input[i]));
		ft_memmove(input[i], input[i + 1], ft_strlen(input[i + 1]));
		i++;
	}
}

void	pipe_parser(char *in, char **envp)
{
	int count;
	char **input;
	int i;

	input = ft_string_split(in, ' ');
	count = mat_count(input);
	i = 0;
	for (int y = 0; input[y]; y++)
		ft_printf("%s\n", input[y]);
	ft_printf("\n\n");
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
	//for (int y = 0; y < count; y++)
	//	ft_printf("%s\n", input[y]);
	//while (i < count)
	//{
	//	if (input[i] == NULL)
	//	{
	//		ft_printf("yo\n");
			
	//		ft_printf("yo\n");
			
	//	}
	//	else
	//		i++;
	//	ft_printf("yo\n");
	//}
	ft_printf("bababoei\n");
	for (int y = 0; input[y]; y++)
		ft_printf("%s\n", input[y]);
	//input = ft_string_split(input[count], '>');
	pipex(count, input, envp);
}