/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:59:06 by roversch          #+#    #+#             */
/*   Updated: 2025/06/25 20:52:47 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
}

size_t	ft_strmcpy(char **dest, const char *src)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(src);
	i = 0;
	*dest = ft_calloc(len, sizeof(char));
	while (src[i])
	{
		(*dest)[i] = src[i];
		i++;
	}
	(*dest)[i] = '\0';
	return (len);
}

void	free_str(char *str)
{
	free(str);
	str = NULL;
}