/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:59:06 by roversch          #+#    #+#             */
/*   Updated: 2025/07/15 18:32:15 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

size_t	ft_strmcpy(char **dest, const char *src)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(src);
	i = 0;
	*dest = ft_calloc(len + 1, sizeof(char));
	if (!*dest)
		return (0);
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
	if (!cpy)
		return (NULL);
	len = 0;
	while (mat[len])
	{
		cpy[len] = ft_strdup(mat[len]);
		if (!cpy[len])
			return (free_array(cpy), NULL);
		len++;
	}
	return (cpy);
}

unsigned long	ft_atoul(const char *nptr)
{
	unsigned long	result;
	int				i;

	result = 0;
	i = 0;
	while (nptr[i] == ' ' || nptr[i] == '\t' || nptr[i] == '\n'
		|| nptr[i] == '\v' || nptr[i] == '\f' || nptr[i] == '\r')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		if (nptr[i++] == '-')
			return (LONG_MAX + ft_atol(nptr) + 1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		if (result > LONG_MAX)
			return (result);
		i++;
	}
	if (!(nptr[i] == '\0') && !(nptr[i] == ' '))
		return (0);
	return (result);
}

int	file_is_empty(char *path)
{
	int		fd;
	char	buff[2];

	if (!path)
		return (1);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (1);
	if (read(fd, buff, 2) == 0)
		return (close(fd), 1);
	close(fd);
	return (0);
}
