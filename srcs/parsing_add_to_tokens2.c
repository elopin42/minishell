/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_add_to_tokens2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 20:53:18 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 21:59:35 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_plain_part(t_token_part **parts_head, char *str, int start,
		int len)
{
	t_token_part_args	args;

	args.parts_head = parts_head;
	args.str = str;
	args.start = start;
	args.len = len;
	return (add_plain_part(&args));
}

int	process_quoted_part(t_quote_args *a, int start, int len,
		char quote_char)
{
	t_token_part_args	args;

	args.parts_head = a->parts_head;
	args.str = a->tmp;
	args.start = start;
	args.len = len;
	args.quote_char = quote_char;
	return (add_quoted_part(&args));
}

int	handle_quote(t_quote_args *args, int *i)
{
	char (quote_char) = args->tmp[*i];
	*(args->has_quotes) = true;
	if (*i > *(args->start))
	{
		if (!process_plain_part(args->parts_head, args->tmp, *(args->start), *i
				- *(args->start)))
			return (0);
	}
	int (quote_start) = ++(*i);
	while (args->tmp[*i] && args->tmp[*i] != quote_char)
		(*i)++;
	int (quote_end) = *i;
	if (args->tmp[*i] == quote_char)
	{
		if (!process_quoted_part(args, quote_start, quote_end - quote_start,
				quote_char))
			return (0);
		(*i)++;
		*(args->start) = *i;
	}
	else
		*(args->start) = quote_start;
	return (1);
}

void	save_history(void)
{
	write_history(HISTORY_FILE);
}
