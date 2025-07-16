/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:47:59 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/16 16:00:20 by nhendrik         ###   ########.fr       */
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
	int		tot_len;
	int		prev_len;
	int		len;
	char	quote_type;

	tot_len = 0;
	prev_len = 0;
	quote_type = 0;
	if ((cpy->type == t_txt || cpy->type == t_flag))
	{
		while (cpy->txt[tot_len])
		{
			len = find_first_quote_len(&cpy->txt[tot_len], &quote_type);
			tot_len += len;
			if (len == -1)
				len = ft_strlen(&cpy->txt[tot_len]);
			while (len-- > prev_len && *i == -1)
				*i = check_txt(cpy, len);
			if (*i == 0)
				(*i)++;
			if (*i != -1)
			{
				ft_lstadd_next(&cpy, ft_lstnew(ft_substr(&cpy->txt[prev_len],
							*i, ft_strlen(&cpy->txt[prev_len]) - *i), 1));
				cpy->txt = ft_strjoin_free(ft_substr(cpy->txt, 0, prev_len), ft_substr_free(&cpy->txt[prev_len], prev_len, *i), 3);
				if (!cpy->txt)
					return (ft_lstdelone(cpy), 0);
				cpy->next->type = find_type(cpy->next->txt);
				cpy->type = find_type(cpy->txt);
			}
			// while ()
			prev_len = tot_len;
		}
	}
	return (1);
}

int	types(char *str)
{
	if (str[0] == '>' && str[1] && str[1] == '>')
		return (2);
	else if (str[0] == '<' && str[1] && str[1] == '<')
		return (2);
	else if (str[0] == '<')
		return (1);
	else if (str[0] == '|')
		return (1);
	else if (str[0] == '>')
		return (1);
	else
		return (0);
}

int type(char c)
{
	if (c == '<')
		return (1);
	else if (c == '>')
		return (1);
	else if (c == '|')
		return (1);
	else
	 	return (0);
}

int rot(t_input *cpy)
{
	char	*str;
	int		where;
	int		i;
	char	quote_type;

	str = ft_strdup(cpy->txt);
	i = 0;
	quote_type = 0;
	where = 0;
	while (str[i])
	{
		if (!quote_type && str[i + 1])
			where = types(&str[i]);
		else if (!quote_type)
			where = type(str[i]);
		if (!quote_type && str[i] == '\'')
			quote_type = '\'';
		else if (!quote_type && str[i] == '"')
			quote_type = '"';
		else if (quote_type == '\'' && str[i] == '\'')
			quote_type = 0;
		else if (quote_type == '"' && str[i] == '"')
			quote_type = 0;
		if (where)
		{
			if (i != 0)
			{
				ft_lstadd_next(&cpy, ft_lstnew(ft_substr(str, i, ft_strlen(str) - i), 1));
				free_and_null(cpy->txt);
				cpy->txt = ft_substr(str, 0, i);
			}
			if (i == 0)
			{
				ft_lstadd_next(&cpy, ft_lstnew(ft_substr(str, where, ft_strlen(str) - (i + where)), 1));
				free_and_null(cpy->txt);
				cpy->txt = ft_substr(str, 0, where);
			}
			free_and_null(str);
			cpy->next->type = find_type(cpy->next->txt);
			cpy->type = find_type(cpy->txt);
			return (1);
		}
		i++;
	}
	free_and_null(str);
	return (1);
}

t_input	*parse_list(t_input *input)
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
			if (!rot(cpy))
				return (NULL);
		cpy = cpy->next;
	}
	return (input);
}
