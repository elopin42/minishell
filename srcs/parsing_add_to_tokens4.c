/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_add_to_tokens4.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 21:21:18 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 21:25:54 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_tmp(char *tmp, t_token_part **parts_head, bool *has_quotes)
{
	t_quote_args	args;

	int (i) = 0;
	int (start) = 0;
	*parts_head = NULL;
	*has_quotes = false;
	args.parts_head = parts_head;
	args.tmp = tmp;
	args.start = &start;
	args.has_quotes = has_quotes;
	while (tmp[i])
	{
		if (tmp[i] == '\'' || tmp[i] == '\"')
		{
			if (!handle_quote(&args, &i))
				return (0);
			continue ;
		}
		i++;
	}
	if (start < i && !process_plain_part(parts_head, tmp, start, i - start))
		return (0);
	return (1);
}

int	add_token_no_quotes(t_tokens **tokens, char *tmp)
{
	t_tokens	*new_tok;
	char		*copy;

	copy = ft_strdup(tmp);
	if (!copy)
		return (0);
	new_tok = ft_new_token(copy, NOQUOTES, NULL);
	if (!new_tok)
	{
		free(copy);
		return (0);
	}
	ft_tokens_add_back(tokens, new_tok);
	return (1);
}

int	add_token_multi(t_tokens **tokens, t_token_part *parts_head)
{
	t_tokens	*new_tok;

	new_tok = ft_new_token(NULL, MULTI, parts_head);
	if (!new_tok)
		return (0);
	ft_tokens_add_back(tokens, new_tok);
	return (1);
}

int	finalize_tokens(t_tokens **tokens, char *tmp, t_token_part *parts_head,
		bool has_quotes)
{
	int	res;

	if (!has_quotes)
		res = add_token_no_quotes(tokens, tmp);
	else
		res = add_token_multi(tokens, parts_head);
	if (!has_quotes && parts_head)
		free_token_parts(&parts_head);
	return (res);
}

int	add_no_quotes_to_tokens(t_tokens **tokens, char *tmp)
{
	t_token_part	*parts_head;
	bool			has_quotes;

	if (!process_tmp(tmp, &parts_head, &has_quotes))
		return (0);
	return (finalize_tokens(tokens, tmp, parts_head, has_quotes));
}
