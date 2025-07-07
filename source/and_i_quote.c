/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_i_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:29:22 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/03 17:40:37 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <complex.h>
#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>
#include <unistd.h>

int	has_char(char *str, char c)
{
	int	i;

	if (!str)
		return (-1);
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

char	find_first_quote(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return ('\'');
		if (str[i] == '"')
			return ('"');
		i++;
	}
	return (0);
}

char	*trim_quotes(char *str, char quote, int start)
{
	char	*ret;

	if (!str)
		return (NULL);
	ret = str;
	ft_memmove(&ret[start], &ret[start + 1], ft_strlen(&ret[start]));
	while (str[start])
	{
		if (str[start] == quote)
		{
			ft_memmove(&ret[start], &ret[start + 1], ft_strlen(&ret[start]));
			break ;
		}
		start++;
	}
	return (ret);
}

int	sub_quote(t_input **input, int *len, int retval, char **env)
{
	int	len2;
	int	quote_type;

	quote_type = find_first_quote(&(*input)->txt[*len]);
	if (!quote_type)
		return (0);
	if (quote_type == '"' && (*input)->txt[*len] == '$')
		(*input)->txt = handle_wildcard((*input)->txt, env, retval, 0);
	*len = has_char(&(*input)->txt[*len], quote_type) + *len;
	len2 = has_char(&(*input)->txt[*len + 1], quote_type);
	if (len2 == -1)
		return (ft_putstr_fd("Invalid input, unclosed quote\n", 2), -1);
	(*input)->txt = trim_quotes((*input)->txt, quote_type, *len);
	*len += len2;
	if (has_char((*input)->txt, '$') >= 0 && quote_type == '"')
		(*input)->txt = handle_wildcard((*input)->txt, env, retval, 0);
	if (*input && (*input)->txt && has_char(&(*input)->txt[*len], '\'') < 0
		&& has_char(&(*input)->txt[*len], '"') < 0)
	{
		(*input) = (*input)->next;
		*len = 0;
	}
	return (1);
}

t_input	**dequote(char **env, int retval, t_input **input)
{
	int		len;
	int		sub;
	t_input	*head;

	len = 0;
	head = *input;
	while (*input && ((*input)->type != t_pipe))
	{
		sub = sub_quote(input, &len, retval, env);
		if (sub == -1)
			return (NULL);
		else if (!sub)
		{
			if (has_char((*input)->txt, '$') >= 0)
				(*input)->txt = handle_wildcard((*input)->txt, env, retval, 0);
			*input = (*input)->next;
			len = 0;
		}
	}
	*input = head;
	return (input);
}
