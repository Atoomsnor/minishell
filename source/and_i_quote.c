/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_i_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:29:22 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/26 13:03:38 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*grok_is_this_true()
{

}

int has_char(char *str, char c)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

t_exec *dequote(t_exec *exec)
{
	int i;
	int	j;
	int found;

	i = 0;
	while (exec->full_cmd[i])
	{
		if (has_char(exec->full_cmd[i], '"'))
		{
			found = 0;
			j = i + 1;
			while (exec->full_cmd[j])
			{
				if (has_char(exec->full_cmd[i], '"'))
					found = 1;
			}
			if (!found)
				//heredoc / dquote
			;
		}
		if (has_char(exec->full_cmd[i], '\''))
		{
			found = 0;
			j = i + 1;
			while (exec->full_cmd[j])
			{
				if (has_char(exec->full_cmd[i], '\''))
					found = 1;
			}
			if (!found)
				//heredoc / dquote
			;
		}
		i++;
	}
	return (exec);
}