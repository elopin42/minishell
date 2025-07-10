/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens_by_quotes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:02:51 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/13 00:19:33 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" // ou adapte à tes fonctions
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

t_token_part	*new_token_part(char *content, t_quote_type quote, bool expand)
{
	t_token_part	*part;

	part = malloc(sizeof(t_token_part));
	if (!part)
		return (NULL);
	part->content = content;
	part->quote = quote;
	part->expand = expand;
	part->next = NULL;
	return (part);
}

void	append_token_part(t_token_part **head, t_token_part *new_part)
{
	t_token_part	*tmp;

	tmp = *head;
	if (!*head)
	{
		*head = new_part;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_part;
}

t_token_part	*dup_parts(t_token_part *parts)
{
	t_token_part	*ret;
	t_token_part	*new_part;
	char			*new;

	ret = NULL;
	while (parts)
	{
		new = ft_strdup(parts->content);
		if (!new)
		{
			free_token_parts(&ret);
			return (NULL);
		}
		new_part = new_token_part(new, parts->quote, parts->expand);
		if (!new_part)
		{
			free_token_parts(&ret);
			return (NULL);
		}
		append_token_part(&ret, new_part);
		parts = parts->next;
	}
	return (ret);
}

void	free_token_parts(t_token_part **head)
{
	t_token_part	*tmp;

	while (*head)
	{
		tmp = (*head)->next;
		if ((*head)->content)
			free((*head)->content);
		if (*head)
			free(*head);
		*head = tmp;
	}
}

void	recursive_call_for_redir(t_parser *p, t_env *ms)
{
	t_tokens	*tmp;

	p->node->left = get_ast(&p->head, ms);
	if (p->right_tokens)
	{
		tmp = p->right_tokens;
		if (p->right_tokens->prev)
			p->right_tokens->prev->next = NULL;
		tmp->prev = NULL;
		p->node->right = get_ast(&tmp, ms);
	}
	else
		p->node->right = NULL;
}
