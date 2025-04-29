/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:11:18 by nhendrik          #+#    #+#             */
/*   Updated: 2025/04/29 16:12:59 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int has_type(t_input *input, t_type type)
{
	t_input *cpy;

	if (input)
	{
		cpy = input;
		while (cpy)
		{
			if (cpy->type == type)
				return (1);
			cpy = cpy->next;
		}
	}
	return (0);
}