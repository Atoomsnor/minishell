/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:14:32 by nhendrik          #+#    #+#             */
/*   Updated: 2025/04/28 13:51:43 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_type find_type(char *in)
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

t_input *mat_to_list(char **mat)
{
	t_input	*input;
	int		i;

	i = 0;
	input = ft_lstnew(mat[0]);
	if (!input)
		return (NULL);
	while(mat[i])
	{
		ft_lstadd_back(&input, ft_lstnew(mat[i]));
		i++;
	}
	return (input);
}

t_input	*parse_list(t_input *input)
{
	t_input *cpy;
	int		count;
	int		i;

	i = 0;
	cpy = input;
	while (cpy)
	{
		cpy->type = find_type(cpy->txt);
		cpy = cpy->next;
	}
	// if (ret[1][0] != '|' && ret[1][0] != '>')
	// {
	// 	ret[0] = ft_strjoin(ret[0], " ");
	// 	ret[0] = ft_strjoin(ret[0], ret[1]);
	// 	move_list(&ret[1], count);
	// 	count--;
	// }
	// while (ret[i])
	// {
	// 	if (ft_strncmp(ret[i], "<", 2) || ft_strncmp(ret[i], "|", 2) ||
	// 			ft_strncmp(ret[i], ">", 2) || ft_strncmp(ret[i], "<<", 3)
	// 			|| ft_strncmp(ret[i], "<<", 3))
	// 	{
	// 		ret[i + 1] = ft_strjoin(ret[i + 1], " ");
	// 		ret[i + 1] = ft_strjoin(ret[i + 1], ret[i + 2]);
	// 		move_list(&ret[i + 2], count - i - 2);
	// 		count--;
	// 		move_list(&ret[i], count - i);
	// 		count--;
	// 	}
	// 	i++;
	// }
	return (input);
}

t_input *init_list(char *in)
{
	t_input *input;
	char	**ret;
	int count;

	ret = ft_string_split(in, ' ');
	mat_to_list(ret);
	
}