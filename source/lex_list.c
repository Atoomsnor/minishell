/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:47:59 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/16 16:37:35 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_type	find_type(char *in)
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

static int	types(char *str)
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

static void	find_quote_type(char *str, char *quote_type, int *where, int i)
{
	if (!*quote_type && str[i + 1])
		*where = types(&str[i]);
	else if (!*quote_type)
		*where = types(&str[i]);
	if (!*quote_type && str[i] == '\'')
		*quote_type = '\'';
	else if (!*quote_type && str[i] == '"')
		*quote_type = '"';
	else if (*quote_type == '\'' && str[i] == '\'')
		*quote_type = 0;
	else if (*quote_type == '"' && str[i] == '"')
		*quote_type = 0;
}

static int	cpy_to_next(t_input *cpy, char *str, int where, int i)
{
	if (i != 0)
	{
		ft_lstadd_next(&cpy, ft_lstnew(ft_substr(
					str, i, ft_strlen(str) - i), 1));
		if (!cpy->next || !cpy->next->txt)
			return (malloc_error_free(free_and_null(str)), 0);
		free_and_null(cpy->txt);
		cpy->txt = ft_substr_free(str, 0, i);
		if (!cpy->txt)
			return (malloc_error_free(free_and_null(str)), 0);
	}
	if (i == 0)
	{
		ft_lstadd_next(&cpy, ft_lstnew(ft_substr(
					str, where, ft_strlen(str) - (i + where)), 1));
		if (!cpy->next || !cpy->next->txt)
			return (malloc_error_free(free_and_null(str)), 0);
		free_and_null(cpy->txt);
		cpy->txt = ft_substr_free(str, 0, where);
		if (!cpy->txt)
			return (malloc_error_free(free_and_null(str)), 0);
	}
	cpy->next->type = find_type(cpy->next->txt);
	cpy->type = find_type(cpy->txt);
	return (1);
}

int	rotation(t_input *cpy)
{
	char	*str;
	int		where;
	int		i;
	char	quote_type;

	str = ft_strdup(cpy->txt);
	if (!str)
		return (malloc_error_free(NULL), 0);
	i = 0;
	quote_type = 0;
	where = 0;
	while (str[i])
	{
		find_quote_type(str, &quote_type, &where, i);
		if (where)
			return (cpy_to_next(cpy, str, where, i));
		i++;
	}
	free_and_null(str);
	return (1);
}
