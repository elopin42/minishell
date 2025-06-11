/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_add_to_tokens.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:02:18 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 21:26:32 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	add_quotes_to_tokens(t_tokens **tokens, char *tmp, char *to_trim,
		t_token_type type)
{
	char			*tmp2;
	t_token_part	*parts;
	t_tokens		*new_tok;

	if (((type == SQUOTES && is_one_squotes(tmp)) && !ft_strchr(tmp, '$'))
		|| ((type == DQUOTES && is_one_dquotes(tmp)) && !ft_strchr(tmp, '$')))
	{
		tmp2 = ft_strtrimv2(tmp, to_trim);
		if (!tmp2)
			return (0);
		new_tok = ft_new_token(tmp2, type, NULL);
		if (!new_tok)
			return (free(tmp2), 0);
		ft_tokens_add_back(tokens, new_tok);
		return (1);
	}
	parts = split_token_by_quotes(tmp);
	if (!parts)
		return (0);
	new_tok = ft_new_token(NULL, MULTI, parts);
	if (!new_tok)
		return (free(parts), 0);
	ft_tokens_add_back(tokens, new_tok);
	return (1);
}

t_token_part	*create_token_part(char *content, t_quote_type quote,
		bool expand)
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

void	add_part_to_list(t_token_part **head, t_token_part *new_part)
{
	t_token_part	*current;

	if (!*head)
	{
		*head = new_part;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_part;
}
