/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:25:23 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/17 01:30:47 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

void	*free_array(char **array)
{
	int	i;

	if (!array)
		return (NULL);
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	return (NULL);
}

char	*ft_strjoin_free(char *s1, char *s2, int liberate)
{
	char	*str;
	size_t	len;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len = len1 + len2;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str)
	{
		ft_strlcpy(str, s1, len1 + 1);
		ft_strlcat(str, s2, len + 1);
	}
	if (liberate == 1 || liberate == 3)
		free(s1);
	if (liberate == 2 || liberate == 3)
		free(s2);
	return (str);
}

char	*ft_strmalloc_free(char *s1, char *s2, int liberate)
{
	char	*str;
	size_t	len;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len = len1 + len2;
	str = NULL;
	if (str)
	{
		ft_strlcpy(str, s1, len1 + 1);
		ft_strlcat(str, s2, len + 1);
	}
	if (liberate == 1 || liberate == 3)
		free(s1);
	if (liberate == 2 || liberate == 3)
		free(s2);
	return (str);
}

void	*free_and_null(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return (NULL);
}

void	*malloc_error_free(void *f)
{
	(void)f;
	ft_putstr_fd("Memory allocation error\n", 2);
	return (NULL);
}
