/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 14:57:39 by roversch          #+#    #+#             */
/*   Updated: 2025/04/15 12:40:05 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*destination;
	const char	*source;

	destination = (char *)dest;
	source = (const char *)src;
	if (dest == 0 && src == 0)
		return (NULL);
	if (n == 0)
		return (dest);
	if (destination <= source)
	{
		while (n--)
			*destination++ = *source++;
	}
	else
	{
		destination += n - 1;
		source += n - 1;
		while (n--)
			*(destination--) = *(source--);
	}
	return (dest);
}
