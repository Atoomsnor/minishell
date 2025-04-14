/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/04/14 16:55:01 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <stdio.h>

int mat_count(char **mat)
{
	int i;

	i = 0;
	while (mat[i])
		i++;
	return (i);
}

int main (int argc, char **argv, char **envp)
{
	char **input;
	int	 count;
	char *in;

	(void)argc;
	(void)argv;
	input = NULL;
	in = NULL;
	while (1)
	{
		count = 0;
		in = readline("gimme: ");
		ft_printf("%s\n", in);
		input = ft_string_split(in, ' ');
		for (int i = 0; input[i]; i++)
			ft_printf("%s\n", input[i]);
		count = mat_count(input);
		pipex(count + 1, input, envp);
	}
}