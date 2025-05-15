/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:14:32 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/15 14:33:13 by nhendrik         ###   ########.fr       */
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

int check_txt(t_input *input, int i)
{
	int len;

	len = ft_strlen(input->txt) - 1;
	if (!ft_strncmp(&input->txt[len - i], ">>", 2))
		return (len - i);
	else if (!ft_strncmp(&input->txt[len - i], "<<", 2))
		return (len - i);
	else if (!ft_strncmp(&input->txt[len - i], "<", 1))
		return (len - i);
	else if (!ft_strncmp(&input->txt[len - i], "|", 1))
		return (len - i);
	else if (!ft_strncmp(&input->txt[len - i], ">", 1))
		return (len - i);
	else
		return (-1);
}

t_input	**matrix_to_list(char **matrix)
{
	t_input	**input;
	int		i;

	input = ft_calloc(1, sizeof(struct t_input *));
	(*input) = ft_lstnew(matrix[0]);
	if (!(*input))
		return (NULL);
	i = 1;
	while (matrix[i])
	{
		ft_lstadd_back(input, ft_lstnew(matrix[i]));
		i++;
	}
	return (input);
}

void printlist(t_input *c)
{
	printf("---START---\n");
	while (c)
	{
		printf("%s\n", c->txt);
		c = c->next;
	}
	printf("---END---\n");

}

t_input	*parse_list(t_input *input)
{
	t_input	*cpy;
	int		i;
	int		len;

	cpy = input;
	while (cpy)
	{
		cpy->type = find_type(cpy->txt);
		cpy = cpy->next;
	}
	cpy = input;
	while (cpy)
	{
		i = -1;
		if (cpy->type == t_txt || cpy->type == t_flag)
		{
			len = ft_strlen(cpy->txt);
			while (len-- > 0 && i == -1)
				i = check_txt(cpy, len);
			if (i == 0)
				i++;
			if (i != -1)
			{
				ft_lstadd_next(&cpy, ft_lstnew(ft_substr(cpy->txt,  i, ft_strlen(cpy->txt) - i)));
				cpy->txt = ft_substr(cpy->txt, 0, i);
				cpy->next->type = find_type(cpy->next->txt);
				cpy->type = find_type(cpy->txt);
			}
		}
		cpy = cpy->next;
	}
	printlist(input);
	return (input);
}

t_input	**init_list(char *in)
{
	char	**matrix;
	t_input **input;

	matrix = ft_string_split(in, ' ');
	input = matrix_to_list(matrix);
	parse_list(input[0]);
	return (input);
}
