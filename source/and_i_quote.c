/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_i_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:29:22 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/28 14:30:34 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>

// char	*grok_is_this_true()
// {

// }

int has_char(char *str, char c)
{
	int i;

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

char find_first_quote(char *str)
{
	int i;

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

char *trim_quotes(char *str, char quote, int start)
{
	char *ret;

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

void quotesiginthandyman(int signal)
{
	if (signal == SIGINT)
	{
		g_signalreceived = signal;
		rl_replace_line("", 0);
		printf("\n");
	}
}

t_exec *dequote(t_exec *exec, char **env, int retval)
{
	int		i;
	int		len;
	int		len2;
	char	quote_type;
	char	*quote;

	i = 0;
	len = 0;
	quote_type = 0;
	len2 = 0;
	quote = NULL;
	while (exec->full_cmd[i])
	{
		quote_type = find_first_quote(&exec->full_cmd[i][len]);
		if (quote_type)
		{
			len = has_char(exec->full_cmd[i], quote_type);
			len2 = has_char(&exec->full_cmd[i][len + 1], quote_type);
			if (len2 != -1)
			{
				exec->full_cmd[i] = trim_quotes(exec->full_cmd[i], quote_type, len);
				len += len2 + 1;
				if (has_char(exec->full_cmd[i], '$') >= 0 && quote_type == '"')
					exec->full_cmd[i] = handle_wildcard(exec->full_cmd[i], env, retval);
			}
			else 
			{
				signal(SIGINT, quotesiginthandyman);
				while (!g_signalreceived && has_char(quote, quote_type) < 0)
				{
					quote = readline("dquote>");
					if (quote[0] == '\0')
						exec->full_cmd[i] = ft_strjoin(exec->full_cmd[i], "\\n");
					else
						exec->full_cmd[i] = ft_strjoin(exec->full_cmd[i], quote);
				}
				g_signalreceived = 0;
				signal(SIGINT, sigint_handler);
				i++;
				len = 0;
			}
		}
		else
		{
			if (has_char(exec->full_cmd[i], '$') >= 0)
					exec->full_cmd[i] = handle_wildcard(exec->full_cmd[i], env, retval);
			i++;
			len = 0;
		}
	}
	return (exec);
}