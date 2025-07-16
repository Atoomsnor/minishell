/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:14:32 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/16 16:39:04 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

static char	*check_syntax(t_input *input)
{
	while (input)
	{
		if (input->type != t_txt && input->type != t_flag)
		{
			if (!input->next)
				return (ft_strdup("newline'\n"));
			else if (input->next->type != t_txt && input->next->type != t_flag
				&& input->type != t_pipe)
				return (ft_strjoin(input->next->txt, "'\n"));
			else if (input->type == t_pipe && input->next->type == t_pipe)
				return (ft_strdup("|'\n"));
		}
		input = input->next;
	}
	return (NULL);
}

static t_input	*parse_list(t_input *input)
{
	t_input	*cpy;

	cpy = input;
	while (cpy)
	{
		if (cpy->type == t_none)
			cpy->type = find_type(cpy->txt);
		cpy = cpy->next;
	}
	cpy = input;
	while (cpy)
	{
		if (cpy->type == t_txt || cpy->type == t_flag)
			if (!rotation(cpy))
				return (NULL);
		cpy = cpy->next;
	}
	return (input);
}

static t_input	**matrix_to_list(char **matrix)
{
	t_input	**input;
	int		i;

	input = ft_calloc(1, sizeof(struct t_input *));
	if (!input)
		return (malloc_error_free(NULL));
	(*input) = ft_lstnew(matrix[0], 0);
	if (!(*input))
		return (malloc_error_free(free_and_null(input)));
	i = 1;
	while (matrix[i])
	{
		if (!ft_lstadd_back(input, ft_lstnew(matrix[i], 0)))
			return (malloc_error_free(shank_input(input)));
		i++;
	}
	return (input);
}

t_input	**init_list(char *in, int skip)
{
	t_input	**input;
	char	**matrix;
	char	*err;

	matrix = ft_string_split(in, ' ');
	if (!matrix)
		return (free_and_null(in));
	input = matrix_to_list(matrix);
	free_array(matrix);
	if (!input)
		return (free_and_null(in));
	if (skip)
		free_and_null(in);
	parse_list(input[0]);
	err = check_syntax(input[0]);
	if (err)
		return (die(NULL, input,
				ft_strjoin_free("syntax error near unexpected token `",
					err, 2), NULL));
	return (input);
}
