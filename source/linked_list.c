/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:08:29 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/08 17:01:30 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

t_input	*ft_lstnew(void *content, int i)
{
	t_input	*node;

	node = (t_input *)malloc(sizeof(t_input));
	if (!node)
	{
		if (content && i)
			free_and_null(content);
		return (NULL);
	}
	if (content)
		node->txt = ft_strdup((char *)content);
	if (content && i)
		free_and_null(content);
	if (content && !node->txt)
		return (NULL);
	node->head = node;
	node->next = NULL;
	node->prev = NULL;
	node->type = t_none;
	node->hd_fd = 0;
	return (node);
}

t_input	*ft_lstlast(t_input *lst)
{
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

int	ft_lstadd_back(t_input **lst, t_input *new_lst)
{
	t_input	*end;

	if (new_lst)
		return (0);
	if (new_lst->next && new_lst->prev)
	{
		new_lst->next->prev = new_lst->prev;
		new_lst->prev->next = new_lst->next;
	}
	else if (new_lst->next)
		new_lst->next->prev = NULL;
	else if (new_lst->prev)
		new_lst->prev->next = NULL;
	end = ft_lstlast(*lst);
	end->next = new_lst;
	new_lst->prev = end;
	new_lst->next = NULL;
	new_lst->head = (*lst)->head;
	return (1);
}
