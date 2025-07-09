/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:14:32 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/08 17:41:11 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

char	find_first_quote_len(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return (i + 1);
		if (str[i] == '"')
			return (i + 1);
		i++;
	}
	return (-1);
}

static int	token_pos(int pos)
{
	if (pos == 0)
		return (2);
	else
		return (0);
}

int	check_txt(t_input *input, int i)
{
	int	len;

	len = find_first_quote_len(input->txt) - 1;
	if (len == -2)
		len = ft_strlen(input->txt) - 1;
	if (!ft_strncmp(&input->txt[len - i], ">>", 2))
		return (len - i + token_pos(len - i));
	else if (!ft_strncmp(&input->txt[len - i], "<<", 2))
		return (len - i + token_pos(len - i));
	else if (!ft_strncmp(&input->txt[len - i], "<", 1))
		return (len - i);
	else if (!ft_strncmp(&input->txt[len - i], "|", 1))
		return (len - i);
	else if (!ft_strncmp(&input->txt[len - i], ">", 1))
		return (len - i);
	else
		return (-1);
}

t_input	**init_list(char *in)
{
	t_input	**input;
	char	**matrix;

	matrix = ft_string_split(in, ' ');
	if (!matrix)
		return (free_and_null(in));
	input = matrix_to_list(matrix);
	free_array(matrix);
	if (!input)
		return (free_and_null(in));
	parse_list(input[0]);
	if (ft_lstlast(*input)->type == t_pipe)
	{
		free(in);
		return (die(NULL, input, ft_strdup("Invalid input\n"), NULL));
	}
	return (input);
}
