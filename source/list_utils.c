/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:13:12 by nhendrik          #+#    #+#             */
/*   Updated: 2025/04/28 15:09:39 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

void	ft_lstsethead(t_input **lst, t_input *head)
{
	t_input	*start;

	if (*lst)
		start = (*lst);
	while (*lst)
	{
		(*lst)->head = head;
		(*lst) = (*lst)->next;
	}
	if (start)
		(*lst) = start;
}

void	ft_lstdelone(t_input *lst)
{
	if (!lst)
		return ;
	if (lst->next && lst->prev)
	{
		lst->next->prev = lst->prev;
		lst->prev->next = lst->next;
	}
	else if (lst->next)
		lst->next->prev = NULL;
	else if (lst->prev)
		lst->prev->next = NULL;
	if (lst == lst->head)
		ft_lstsethead(&lst, lst->next);
	if (lst->txt)
		free(lst->txt);
	free(lst);
}

void	ft_lstadd_next(t_input **lst, t_input *new_lst)
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
	new_lst->prev = (*lst);
	new_lst->next = NULL;
	new_lst->head = (*lst)->head;
	if ((*lst)->next)
	{
		new_lst->next = (*lst)->next;
		(*lst)->next->prev = new_lst;
	}
	(*lst)->next = new_lst;
}


t_input	*list_move(t_input *lst, int times)
{
	t_input	*cpy;

	cpy = lst;
	while (times > 0 && cpy)
	{
		cpy = cpy->next;
		times--;
	}
	return (cpy);
}