/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens_norme.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 20:12:17 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 21:51:43 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_content(t_token_part *p, t_env *ms)
{
	t_tokens	*tmp;
	char		*expanded;

	if (p->expand)
	{
		tmp = malloc(sizeof(t_tokens));
		if (!tmp)
			exit_clean(ms, MALLOC_ERROR);
		tmp->token = ft_strdup(p->content);
		if (!tmp->token)
			exit_clean(ms, MALLOC_ERROR);
		tmp->next = NULL;
		tmp->prev = NULL;
		tmp->parts = NULL;
		tmp->type = NOQUOTES;
		search_and_destroy(&tmp, ms);
		expanded = ft_strdup(tmp->token);
		free(tmp->token);
		free(tmp);
	}
	else
		expanded = ft_strdup(p->content);
	if (!expanded)
		exit_clean(ms, MALLOC_ERROR);
	return (expanded);
}

static bool	all_parts_empty(t_token_part *parts)
{
	t_token_part	*check;

	check = parts;
	while (check)
	{
		if (check->content && check->content[0] != '\0')
			return (false);
		check = check->next;
	}
	return (true);
}

static char	*expand_token_part(t_token_part **p, t_env *ms)
{
	char	*expanded;

	if ((*p)->content && !ft_strcmp((*p)->content, "$") && (*p)->next
		&& (*p)->next->quote == DOUBLE_QUOTE)
	{
		expanded = ft_strdup((*p)->next->content);
		(*p) = (*p)->next->next;
	}
	else
	{
		expanded = expand_content(*p, ms);
		(*p) = (*p)->next;
	}
	if (!expanded)
		exit_clean(ms, MALLOC_ERROR);
	return (expanded);
}

char	*expand_parts(t_token_part *parts, t_env *ms)
{
	char			*final;
	char			*expanded;
	char			*new_final;
	t_token_part	*p;

	if (all_parts_empty(parts))
		return (ft_strdup(""));
	final = NULL;
	p = parts;
	while (p)
	{
		expanded = expand_token_part(&p, ms);
		if (!final)
			new_final = ft_strjoin("", expanded);
		else
			new_final = ft_strjoin(final, expanded);
		free(final);
		free(expanded);
		if (!new_final)
			exit_clean(ms, MALLOC_ERROR);
		final = new_final;
	}
	return (final);
}

int	ft_seek_index(char *s, char c)
{
	int	r;

	r = 0;
	while (s[r])
	{
		if (s[r] == c)
		{
			if (s[r + 1] == 0)
				return (-1);
			return (r);
		}
		r++;
	}
	return (-1);
}
