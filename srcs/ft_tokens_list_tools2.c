/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_list_tools2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:00:31 by tbeauman          #+#    #+#             */
/*   Updated: 2025/05/04 17:05:13 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_clear_left_tokens(t_tokens **lst, void (*del)(void *))
{
	t_tokens	*tmp;

	while (*lst)
	{
		tmp = (*lst)->prev;
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

