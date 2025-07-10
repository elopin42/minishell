/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tools2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 20:40:10 by elopin            #+#    #+#             */
/*   Updated: 2025/06/13 00:19:28 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_quote_type	get_quote_type(char c)
{
	if (c == '\'')
		return (SINGLE_QUOTE);
	return (DOUBLE_QUOTE);
}

void	cut_chain_for_redir(t_parser *p, t_env *ms)
{
	p->file_token = p->tokens->next;
	p->right_tokens = NULL;
	p->left_tokens = p->tokens->prev;
	if (p->file_token)
	{
		p->node->file = p->file_token->token;
		p->right_tokens = p->file_token->next;
		if (p->right_tokens)
			p->right_tokens->prev = NULL;
		p->file_token->prev = NULL;
		p->file_token->next = NULL;
		p->node->file_token = p->file_token;
	}
	else
		set_error(ms, SYNTAX_ERROR);
	if (p->left_tokens)
		p->left_tokens->next = NULL;
	else
		p->head = NULL;
	append_arguments(p, ms);
	p->node->cmd = p->tokens;
	if (p->node->cmd)
		trunc_node_cmd(p);
}

static bool	handle_quoted(const char *str, int *i, t_token_part **parts)
{
	char			*sub;
	t_token_part	*new;

	char (quote_char) = str[*i];
	t_quote_type(current_quote) = get_quote_type(quote_char);
	bool(expand) = (current_quote == DOUBLE_QUOTE);
	(*i)++;
	int (start) = *i;
	while (str[*i] && str[*i] != quote_char)
		(*i)++;
	if (str[*i] != quote_char)
		return (true);
	sub = ft_substr(str, (unsigned int)start, (size_t)(*i - start));
	if (!sub)
		return (free_token_parts(parts), false);
	if (!(sub[0] == '\0' && current_quote == SINGLE_QUOTE))
	{
		new = new_token_part(sub, current_quote, expand);
		if (!new)
			return (free_token_parts(parts), false);
		append_token_part(parts, new);
	}
	else
		free(sub);
	return ((*i)++, true);
}

static bool	handle_unquoted(const char *str, int *i, t_token_part **parts)
{
	int				start;
	char			*sub;
	t_token_part	*new;

	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	if (*i - start > 0)
	{
		sub = ft_substr(str, (unsigned int)start, (size_t)(*i - start));
		if (!sub)
			return (free_token_parts(parts), false);
		new = new_token_part(sub, NONE, true);
		if (!new)
			return (free_token_parts(parts), false);
		append_token_part(parts, new);
	}
	return (true);
}

t_token_part	*split_token_by_quotes(const char *str)
{
	t_token_part	*parts;
	int				i;

	parts = NULL;
	i = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && strchr(&str[i + 1], str[i]))
		{
			if (!handle_quoted(str, &i, &parts))
				return (NULL);
		}
		else
		{
			if (!handle_unquoted(str, &i, &parts))
				return (NULL);
		}
	}
	return (parts);
}
