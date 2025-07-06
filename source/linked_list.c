/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:08:29 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/11 13:34:17 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

int	ft_lstsize(t_input *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

t_input	*ft_lstnew(void *content, int i)
{
	t_input	*node;

	node = (t_input *)malloc(sizeof(t_input));
	if (!node)
		return (NULL);
	node->txt = ft_strdup((char *)content);
	if (content && i)
		free_and_null(content);
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

void	ft_lstadd_back(t_input **lst, t_input *new_lst)
{
	t_input	*end;

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
}

void	ft_lstadd_front(t_input **lst, t_input *new_lst)
{
	if (lst)
	{
		if ((*lst) != new_lst)
		{
			if (new_lst->next && new_lst->prev)
			{
				new_lst->next->prev = new_lst->prev;
				new_lst->prev->next = new_lst->next;
			}
			else if (new_lst->next)
				new_lst->next->prev = NULL;
			else if (new_lst->prev)
				new_lst->prev->next = NULL;
			new_lst->next = *lst;
			new_lst->prev = NULL;
			if (*lst)
				(*lst)->prev = new_lst;
			*lst = new_lst;
		}
	}
}
