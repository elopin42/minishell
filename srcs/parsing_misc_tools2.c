/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_misc_tools2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:51:41 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/13 00:25:48 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	add_to_tokens(t_tokens **tokens, char *tmp, char *to_trim,
		t_token_type type)
{
	char	*tmp2;

	tmp2 = ft_strtrimv2(tmp, to_trim);
	if (!tmp2)
		return (0);
	ft_tokens_add_back(tokens, ft_new_token(tmp2, type, NULL));
	return (1);
}

bool	token_stops(char *line, int i, bool in_dquotes, bool in_quotes)
{
	if ((line[i] == ' ' || is_operand2(&line[i]) || is_operand1(&line[i]))
		&& !in_dquotes && !in_quotes)
		return (1);
	return (0);
}

void	handle_specials(char *line, int i, t_tknz_bools *t, t_env *ms)
{
	(void)ms;
	if (line[i] == '\"' && !t->in_quotes)
		t->in_dquotes = !t->in_dquotes;
	if (line[i] == '\'' && !t->in_dquotes)
		t->in_quotes = !t->in_quotes;
	if (line[i] == '(' && !t->in_dquotes && !t->in_quotes)
		t->parenthesis_depth++;
	if (line[i] == ')' && !t->in_dquotes && !t->in_quotes)
		t->parenthesis_depth--;
}

int	check_quotes_ended(t_tknz_bools t, t_env *ms)
{
	if (t.in_quotes || t.in_dquotes || t.parenthesis_depth)
	{
		set_parse_error(ms, SYNTAX_ERROR);
		return (0);
	}
	return (1);
}

void	ptit_clean_du_cul(char ***argscmd, char **path, t_env *ms, int code)
{
	free_tab(*argscmd);
	free(*path);
	exit_clean(ms, code);
}
