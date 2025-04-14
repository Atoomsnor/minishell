/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:20:30 by roversch          #+#    #+#             */
/*   Updated: 2025/04/14 16:13:04 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include "../../libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

typedef enum t_file_status
{
	READ_ERROR = -1,
	READ_EOF = 0,
}	t_file_status;

char	*get_the_line(int fd, char *leftover, char *buffer);
char	*store_leftovers(char **line);
char	*get_next_line(int fd);
// size_t	ft_strlen(const char *s);
// char	*ft_strjoin(char const *s1, char const *s2);
// char	*ft_substr(char const *s, unsigned int start, size_t len);
// char	*ft_strdup(const char *s);
// char	*ft_strchr(char *s, int c);

#endif