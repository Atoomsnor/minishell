/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:14:32 by nhendrik          #+#    #+#             */
/*   Updated: 2025/04/28 17:04:21 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
#include <stdio.h>

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

int check_txt(t_input *input)
{
	if (!ft_strncmp(input->txt, "<", 1))
		return (1);
	else if (!ft_strncmp(input->txt, "|", 1))
		return (1);
	else if (!ft_strncmp(input->txt, ">", 1))
		return (1);
	else if (!ft_strncmp(input->txt, ">>", 2))
		return (2);
	else if (!ft_strncmp(input->txt, "<<", 2))
		return (2);
	else
		return (0);
}

t_input	**mat_to_list(char **mat)
{
	t_input	**input;
	int		i;

	(*input) = ft_lstnew(mat[0]);
	if (!(*input))
		return (NULL);
	i = 1;
	while (mat[i])
	{
		ft_lstadd_back(input, ft_lstnew(mat[i]));
		i++;
	}
	return (input);
}

void printlist(t_input *c)
{
	while (c)
	{
		printf("%s\n", c->txt);
		c = c->next;
	}

}

t_input	*parse_list(t_input *input)
{
	t_input	*cpy;
	int		i;

	cpy = input;
	while (cpy)
	{
		cpy->type = find_type(cpy->txt);
		cpy = cpy->next;
	}
	cpy = input;
	while (cpy)
	{
		if (cpy->type == t_txt)
		{
			i = check_txt(cpy);
			if (i)
			{
				ft_lstadd_next(&cpy, ft_lstnew(ft_substr(cpy->txt,  i, ft_strlen(cpy->txt) - i)));
				cpy->txt = ft_substr(cpy->txt, 0, i);
				cpy->next->type = find_type(cpy->next->txt);
			}
		}
		cpy = cpy->next;
	}
	printlist(input);
	// while (cpy)
	// {
		
	// }
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

t_input	**init_list(t_shell *shell)
{
	t_input	*input;
	char	**ret;

	ret = ft_string_split(shell->in, ' ');
	shell->curr_input[0] = mat_to_list(ret);
	parse_list(shell->curr_input[0]);
	return (shell->curr_input);
}
