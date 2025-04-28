/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:13:12 by nhendrik          #+#    #+#             */
/*   Updated: 2025/04/28 14:13:26 by nhendrik         ###   ########.fr       */
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
		lst->prev->next = lst->prev;
	}
	else if (lst->next)
		lst->next->prev = NULL;
	else if (lst->prev)
		lst->prev->next = NULL;
	if (lst->txt)
		free(lst->txt);
	free(lst);
}
