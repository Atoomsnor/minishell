/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_i_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:29:22 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/03 13:58:48 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>

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
			ft_memmove(&ret[start], &ret[start + 1], ft_strlen(&ret[start]));
		start++;
	}
	return (ret);
}

void	quotesiginthandyman(int signal)
{
	if (signal == SIGINT)
	{
		g_signalreceived = signal;
		rl_replace_line("", 0);
		printf("\n");
	}
}

t_input	**dequote(char **env, int retval, t_input **input)
{
	char	*quote;
	char	quote_type;
	int		len;
	int		len2;
	int		i;
	t_input *head;

	quote = NULL;
	quote_type = 0;
	len = 0;
	len2 = 0;
	i = 0;
	head = *input;
	while (*input)
	{
		quote_type = find_first_quote(&(*input)->txt[len]);
		if (quote_type)
		{
			len = has_char((*input)->txt, quote_type);
			len2 = has_char(&(*input)->txt[len + 1], quote_type);
			if (len2 != -1)
			{
				(*input)->txt = trim_quotes((*input)->txt, quote_type, len);
				len += len2 + 1;
				if (has_char((*input)->txt, '$') >= 0 && quote_type == '"')
					(*input)->txt = handle_wildcard((*input)->txt, env, retval);
			}
			else
			{
				signal(SIGINT, quotesiginthandyman);
				while (!g_signalreceived && has_char(quote, quote_type) < 0)
				{
					quote = readline("dquote>");
					if (quote[0] == '\0')
						(*input)->txt = ft_strjoin((*input)->txt, "\\n");
					else
						(*input)->txt = ft_strjoin((*input)->txt, quote);
				}
				g_signalreceived = 0;
				signal(SIGINT, sigint_handler);
				*input = (*input)->next;
				len = 0;
			}
		}
		else
		{
			if (has_char((*input)->txt, '$') >= 0)
				(*input)->txt = handle_wildcard((*input)->txt, env, retval);
			*input = (*input)->next;
			len = 0;
		}
	}
	*input = head;
	return (input);
}
