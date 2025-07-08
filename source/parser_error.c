/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:33:54 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/08 16:36:54 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

void	*adjust_error(char **error_msg, char *err1, char *err2)
{
	if (*error_msg)
	{
		free(*error_msg);
		*error_msg = NULL;
	}
	*error_msg = ft_strjoin(err1, err2);
	return (NULL);
}

void	*set_retval(int *retval, int val)
{
	*retval = val;
	return (NULL);
}
