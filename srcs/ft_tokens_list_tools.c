/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_list_tools.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 10:55:56 by tbeauman          #+#    #+#             */
/*   Updated: 2025/05/06 15:04:40 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_tokens_add_back(t_tokens **lst, t_tokens *new)
{
	t_tokens	*head;

	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	head = *lst;
	while ((*lst)->next)
		*lst = (*lst)->next;
	(*lst)->next = new;
	new->prev = *lst;
	*lst = head;
}

void	ft_tokens_add_front(t_tokens **lst, t_tokens *new)
{
	t_tokens	*head;

	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	head = *lst;
	while ((*lst)->prev)
		*lst = (*lst)->prev;
	(*lst)->prev = new;
	new->prev = NULL;
	*lst = head;
}

void	ft_clear_tokens(t_tokens **lst, void (*del)(void *))
{
	t_tokens	*tmp;

	if (!lst || !del)
		return ;
	while (*lst && (*lst)->prev)
		*lst = (*lst)->prev;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->token && del)
			del((*lst)->token);
		free(*lst);
		(*lst) = tmp;
	}
	*lst = NULL;
}

void	ft_clear_right_tokens(t_tokens **lst, void (*del)(void *))
{
	t_tokens	*tmp;

	while (*lst)
	{
		tmp = (*lst)->next;
		del((*lst)->token);
		free(*lst);
		(*lst) = tmp;
	}
	*lst = NULL;
}
