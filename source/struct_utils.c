/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:11:18 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/27 19:37:37 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdarg.h>
#include <stdio.h>

int	has_type(t_input *input, ...)
{
	t_input	*cpy;
	va_list	ptr;
	t_type	curr;

	if (!input)
		return (0);
	cpy = input;
	while (cpy)
	{
		va_start(ptr, input);
		while (1)
		{
			curr = va_arg(ptr, t_type);
			if (curr == 0)
				break ;
			if (cpy->type == curr)
			{
				va_end(ptr);
				return (1);
			}
		}
		cpy = cpy->next;
		va_end(ptr);
	}
	return (0);
}

void print_strings_fd(int fd, ...)
{
	va_list ptr;
	char	*curr;

	va_start(ptr, fd);
	while (1)
	{
		curr = va_arg(ptr, char *);
		if (curr == NULL)
			break ;
		ft_putstr_fd(curr, fd);
	}
	va_end(ptr);
}
