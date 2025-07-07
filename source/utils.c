/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:59:06 by roversch          #+#    #+#             */
/*   Updated: 2025/07/03 18:09:24 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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

char	**ft_matdup(char **mat)
{
	char	**cpy;
	int		len;

	if (!mat)
		return (NULL);
	len = 0;
	while (mat[len])
		len++;
	cpy = ft_calloc(len + 1, sizeof(char *));
	len = 0;
	while (mat[len])
	{
		cpy[len] = ft_strdup(mat[len]);
		len++;
	}
	return (cpy);
}
