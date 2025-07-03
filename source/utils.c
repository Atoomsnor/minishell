/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:59:06 by roversch          #+#    #+#             */
/*   Updated: 2025/07/03 17:21:14 by nhendrik         ###   ########.fr       */
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

int	ft_max(int a, int b)
{
	if (a == b)
		return (a);
	if (a > b)
		return (a);
	if (b > a)
		return (b);
	return (0);
}

void	free_str(char *str)
{
	free(str);
	str = NULL;
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*new_str;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	new_str = (char *)malloc(i + j + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[j])
		new_str[i++] = s1[j++];
	j = 0;
	while (s2[j])
		new_str[i++] = s2[j++];
	free(s1);
	free(s2);
	new_str[i] = '\0';
	return (new_str);
}

char	*ft_substr_free(char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	s_len;
	size_t	i;

	s_len = 0;
	i = 0;
	if (!s)
		return (NULL);
	while (s[s_len])
		s_len++;
	if (start >= s_len)
		return (ft_strdup(""));
	if (start + len > s_len)
		len = s_len - start;
	substr = (char *)malloc(len + 1);
	if (!substr)
		return (NULL);
	while (i < len)
		substr[i++] = s[start++];
	free(s);
	substr[i] = '\0';
	return (substr);
}

void	free_and_null(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}