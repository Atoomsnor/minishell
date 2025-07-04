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

t_input	**matrix_to_list(char **matrix)
{
	t_input	**input;
	int		i;

	input = ft_calloc(1, sizeof(struct t_input *));
	if (!input)
		return (NULL);
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

void	printlist(t_input *c, int i)
{
	int fd;
	if (i)
		fd = open("outtest", O_RDWR);
	else
		fd = 1;
	printf("---START LIST---\n");
	while (c)
	{
		ft_putendl_fd(c->txt, fd);
		c = c->next;
	}
	printf("---LIST END---\n");
}



t_input	*parse_list(t_input *input)
{
	t_input	*cpy;
	int		i;
	int		len;

	cpy = input;
	while (cpy)
	{
		if (cpy->type == t_none)
			cpy->type = find_type(cpy->txt);
		cpy = cpy->next;
	}
	cpy = input;
	while (cpy)
	{
		i = -1;
		if ((cpy->type == t_txt || cpy->type == t_flag))
		{
			len = find_first_quote_len(cpy->txt);
			if (len == -1)
				len = ft_strlen(cpy->txt);
			while (len-- > 0 && i == -1)
				i = check_txt(cpy, len);
			if (i == 0)
				i++;
			if (i != -1)
			{
				ft_lstadd_next(&cpy, ft_lstnew(ft_substr(cpy->txt, i, ft_strlen(cpy->txt) - i)));
				cpy->txt = ft_substr(cpy->txt, 0, i);
				cpy->next->type = find_type(cpy->next->txt);
				cpy->type = find_type(cpy->txt);
			}
		}
		cpy = cpy->next;
	}
	return (input);
}

void	print_matrix(char **input)
{
	for (int i = 0; input[i]; i++)
		ft_printf("%s\n", input[i]);
	ft_printf("endl\n");
}

t_input	**init_list(char *in)
{
	t_input	**input;
	char	**matrix;
	char	*readl;

	matrix = ft_string_split(in, ' ');
	// print_matrix(matrix);
	input = matrix_to_list(matrix);
	// printlist(*input, 0);
	free_array(matrix);
	parse_list(input[0]);
	// printlist(*input, 0);
	readl = NULL;
	while (ft_lstlast(*input)->type == t_pipe)
	{
		readl = readline("> ");
		matrix = ft_string_split(readl, ' ');
		while (*matrix)
			ft_lstadd_back(input, ft_lstnew(*matrix++));
		parse_list(input[0]);
	}
	return (input);
}
