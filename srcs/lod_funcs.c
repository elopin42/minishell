/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lod_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:41:08 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/09 16:43:39 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_lod	*ft_new_lod(int content, bool close)
{
	t_lod	*ret;

	ret = (t_lod *)malloc(sizeof(t_lod));
	if (!ret)
		return (NULL);
	ret->i = content;
	ret->close = close;
	ret->next = NULL;
	return (ret);
}
void	ft_lod_add_back(t_lod **lst, t_lod *new)
{
	t_lod	*head;

	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	head = *lst;
	while ((*lst)->next)
		*lst = (*lst)->next;
	(*lst)->next = new;
	*lst = head;
}

void	ft_clear_lod(t_lod **lst)
{
	t_lod	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free(*lst);
		(*lst) = tmp;
	}
	*lst = NULL;
}
