/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_add_to_tokens3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 20:58:58 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 21:25:33 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	add_plain_part(t_token_part_args *args)
{
	char			*current_part;
	t_token_part	*new_part;

	current_part = ft_substr(args->str, args->start, args->len);
	if (!current_part)
		return (0);
	new_part = create_token_part(current_part, NONE, true);
	if (!new_part)
	{
		free(current_part);
		return (0);
	}
	add_part_to_list(args->parts_head, new_part);
	return (1);
}

int	add_quoted_part(t_token_part_args *args)
{
	char			*substr;
	t_quote_type	quote_type;
	t_token_part	*new_part;

	substr = ft_substr(args->str, (unsigned int)args->start, (size_t)args->len);
	if (!substr)
		return (0);
	if (args->quote_char == '\'')
		quote_type = SINGLE_QUOTE;
	else
		quote_type = DOUBLE_QUOTE;
	new_part = create_token_part(substr, quote_type,
			(quote_type == DOUBLE_QUOTE));
	if (!new_part)
	{
		free(substr);
		return (0);
	}
	add_part_to_list(args->parts_head, new_part);
	return (1);
}
