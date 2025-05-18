/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_list_tools3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:05:03 by tbeauman          #+#    #+#             */
/*   Updated: 2025/05/04 17:05:12 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tokens	*ft_new_token(void const *content, t_token_type type)
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
		if (!token)
		{
			ft_clear_tokens(&head, &free);
			return (NULL);
		}
		ft_tokens_add_back(&dup, ft_new_token(token, og->type));
		og = og->next;
	}
	og = head;
	return (dup);
}