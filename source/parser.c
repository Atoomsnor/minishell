/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:14:32 by nhendrik          #+#    #+#             */
/*   Updated: 2025/04/24 00:13:12 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token find_token(char *in)
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

char	**parser(char *in)
{
	int		count;
	char	**ret;
	int		i;

	ret = ft_string_split(in, ' ');
	count = mat_count(ret);
	check_io(ret, &count);
	i = 0;
	print_matrix(ret);
	if (ret[1][0] != '|' && ret[1][0] != '>')
	{
		ret[0] = ft_strjoin(ret[0], " ");
		ret[0] = ft_strjoin(ret[0], ret[1]);
		move_list(&ret[1], count);
		count--;
	}
	print_matrix(ret);
	while (ret[i])
	{
		if (ft_strncmp(ret[i], "<\0", 3) || ft_strncmp(ret[i], "|\0", 3) ||
				ft_strncmp(ret[i], ">\0", 3) || ft_strncmp(ret[i], "<<\0", 4)
				|| ft_strncmp(ret[i], "<<\0", 4))
		{
			ret[i + 1] = ft_strjoin(ret[i + 1], " ");
			ret[i + 1] = ft_strjoin(ret[i + 1], ret[i + 2]);
			move_list(&ret[i + 2], count - i - 2);
			count--;
			move_list(&ret[i], count - i);
			count--;
		}
		i++;
	}
	print_matrix(ret);
	return (ret);
}