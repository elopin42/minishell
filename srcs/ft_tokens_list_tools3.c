/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_list_tools3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:05:03 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/12 22:01:07 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tokens	*ft_new_token(void const *content, t_token_type type,
		t_token_part *parts)
{
	t_tokens	*ret;

	ret = (t_tokens *)malloc(sizeof(t_tokens));
	if (!ret)
		return (NULL);
	ret->token = (void *)content;
	ret->type = type;
	ret->parts = parts;
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

t_token_part	*dup_token_parts(t_tokens *og, t_tokens *head, char *token)
{
	t_token_part	*parts;

	parts = NULL;
	if (og->parts)
	{
		parts = dup_parts(og->parts);
		if (!parts)
		{
			if (token)
				free(token);
			ft_clear_tokens(&head, &free);
			return (NULL);
		}
	}
	return (parts);
}

char	*dup_token_str(t_tokens *og, t_tokens *head)
{
	char	*token;

	token = NULL;
	if (og->token)
	{
		token = ft_strdup(og->token);
		if (!token)
		{
			ft_clear_tokens(&head, &free);
			return (NULL);
		}
	}
	return (token);
}

t_tokens	*dup_tokens(t_tokens *og)
{
	t_tokens		*head;
	t_tokens		*dup;
	t_tokens		*new;
	char			*token;
	t_token_part	*parts;

	dup = NULL;
	head = og;
	while (og)
	{
		token = dup_token_str(og, head);
		if (og->token && !token)
			return (ft_clear_tokens(&dup, &free), NULL);
		parts = dup_token_parts(og, head, token);
		if (og->parts && !parts)
			return (free(token), ft_clear_tokens(&dup, &free), NULL);
		new = ft_new_token(token, og->type, parts);
		if (!new)
			return (free(token), ft_clear_tokens(&dup, &free), NULL);
		ft_tokens_add_back(&dup, new);
		og = og->next;
	}
	return (dup);
}
