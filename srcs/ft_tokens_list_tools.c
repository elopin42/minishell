/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_list_tools.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 10:55:56 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/30 17:54:10 by elopin           ###   ########.fr       */
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

	while (*lst && (*lst)->prev)
		*lst = (*lst)->prev;
	while (*lst)
	{
		tmp = (*lst)->next;
		del((*lst)->token);
		free(*lst);
		(*lst) = tmp;
	}
	*lst = NULL;
}

void	ft_del_token(t_tokens *lst, void (*del)(void *))
{
	if (lst)
	{
		del(lst->token);
		free(lst);
		lst = NULL;
	}
}

t_tokens	*ft_new_token(void const *content, t_node_type type)
{
	t_tokens	*ret;

	ret = (t_tokens *)malloc(sizeof(t_tokens));
	if (!ret)
		return (NULL);
	ret->token = (void *)content;
	ret->type = type;
	ret->prev = NULL;
	ret->next = NULL;
	return (ret);
}

int	ft_lst_tokens_size(t_tokens *lst)
{
	int	i;

	i = 0;
	while (lst && lst->prev)
		lst = lst->prev;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

t_tokens	*dup_tokens(t_tokens *og)
{
	t_tokens *head;
	t_tokens *dup;
	char *token;

	dup = NULL;
	head = og;
	while (og)
	{
		token = ft_strdup(og->token);
		ft_tokens_add_back(&dup, ft_new_token(token, og->type));
		og = og->next;
	}
	og = head;
	return (dup);
}