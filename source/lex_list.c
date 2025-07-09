/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:47:59 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/08 16:59:41 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

t_input	**matrix_to_list(char **matrix)
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

static t_type	find_type(char *in)
{
	if (!ft_strncmp(in, "<", 2))
		return (t_left);
	else if (!ft_strncmp(in, "|", 2))
		return (t_pipe);
	else if (!ft_strncmp(in, ">", 2))
		return (t_right);
	else if (!ft_strncmp(in, ">>", 3))
		return (t_append);
	else if (!ft_strncmp(in, "<<", 3))
		return (t_heredoc);
	else if (in[0] == '-')
		return (t_flag);
	else
		return (t_txt);
}

int	rotation(int *i, t_input *cpy)
{
	int	len;

	if ((cpy->type == t_txt || cpy->type == t_flag))
	{
		len = find_first_quote_len(cpy->txt);
		if (len == -1)
			len = ft_strlen(cpy->txt);
		while (len-- > 0 && *i == -1)
			*i = check_txt(cpy, len);
		if (*i == 0)
			(*i)++;
		if (*i != -1)
		{
			ft_lstadd_next(&cpy, ft_lstnew(ft_substr(cpy->txt,
						*i, ft_strlen(cpy->txt) - *i), 1));
			cpy->txt = ft_substr_free(cpy->txt, 0, *i);
			if (!cpy->txt)
				return (ft_lstdelone(cpy), 0);
			cpy->next->type = find_type(cpy->next->txt);
			cpy->type = find_type(cpy->txt);
		}
	}
	return (1);
}

t_input	*parse_list(t_input *input)
{
	t_input	*cpy;
	int		i;

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
		i = -1;
		if (!rotation(&i, cpy))
			return (NULL);
		cpy = cpy->next;
	}
	return (input);
}
