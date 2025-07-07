/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:14:32 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/01 14:07:07 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>

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

char	find_first_quote_len(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return (i + 1);
		if (str[i] == '"')
			return (i + 1);
		i++;
	}
	return (-1);
}

static int	token_pos(int pos)
{
	if (pos == 0)
		return (2);
	else
		return (0);
}

int	check_txt(t_input *input, int i)
{
	int	len;

	len = find_first_quote_len(input->txt) - 1;
	if (len == -2)
		len = ft_strlen(input->txt) - 1;
	if (!ft_strncmp(&input->txt[len - i], ">>", 2))
		return (len - i + token_pos(len - i));
	else if (!ft_strncmp(&input->txt[len - i], "<<", 2))
		return (len - i + token_pos(len - i));
	else if (!ft_strncmp(&input->txt[len - i], "<", 1))
		return (len - i);
	else if (!ft_strncmp(&input->txt[len - i], "|", 1))
		return (len - i);
	else if (!ft_strncmp(&input->txt[len - i], ">", 1))
		return (len - i);
	else
		return (-1);
}

t_input	**init_list(char *in)
{
	t_input	**input;
	char	**matrix;
	char	*readl;

	matrix = ft_string_split(in, ' ');
	input = matrix_to_list(matrix);
	free_array(matrix);
	parse_list(input[0]);
	readl = NULL;
	while (ft_lstlast(*input)->type == t_pipe)
	{
		readl = readline("> ");
		matrix = ft_string_split(readl, ' ');
		while (*matrix)
			ft_lstadd_back(input, ft_lstnew(*matrix++, 1));
		parse_list(input[0]);
	}
	return (input);
}
