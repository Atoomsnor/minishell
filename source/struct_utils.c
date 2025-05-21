/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:11:18 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/08 15:08:54 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdarg.h>
#include <stdio.h>

int has_type(t_input *input, ...)
{
	t_input	*cpy;
	va_list	ptr;
	t_type curr;

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
				break;
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