/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:11:18 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/17 18:21:58 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdarg.h>
#include <stdio.h>

// int	print_strings_fd(int fd, ...)
// {
// 	va_list	ptr;
// 	char	*curr;

// 	va_start(ptr, fd);
// 	while (1)
// 	{
// 		curr = va_arg(ptr, char *);
// 		if (curr == NULL)
// 			break ;
// 		ft_putstr_fd(curr, fd);
// 	}
// 	va_end(ptr);
// 	return (0);
// }
