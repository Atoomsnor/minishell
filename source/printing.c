/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:45:59 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/07 16:45:59 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

//void	printlist(t_input *c, int i)
//{
//	int	fd;

//	if (i)
//		fd = open("outtest", O_RDWR);
//	else
//		fd = 1;
//	printf("---START LIST---\n");
//	while (c)
//	{
//		ft_putendl_fd(c->txt, fd);
//		c = c->next;
//	}
//	printf("---LIST END---\n");
//}

//void	print_matrix(char **input)
//{
//	for (int i = 0; input[i]; i++)
//		ft_printf("%s\n", input[i]);
//	ft_printf("endl\n");
//}

//void	print_exec(t_exec *exec)
//{
//	int	i;

//	i = 0;
//	printf("exec\n");
//	printf("in: %i\nout: %i\nfullpath: %s\n", 
// 			exec->in_fd, exec->out_fd, exec->full_path);
//	while (exec->full_cmd[i])
//	{
//		printf("%s\n", exec->full_cmd[i]);
//		i++;
//	}
//	printf("execend\n");
//}