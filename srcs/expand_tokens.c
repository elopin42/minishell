/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:47:39 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/11 22:20:46 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_token(char *og, char *var, int si, int stop)
{
	int		j;
	int		k;
	char	*ret;

	ret = malloc(ft_strlen(og) + ft_strlen(var) + 2 - (stop - si));
	int (i) = -1;
	if (!ret)
		return (NULL);
	while (++i < si)
		ret[i] = og[i];
	k = i + stop - si;
	j = -1;
	while (var[++j])
	{
		ret[i] = var[j];
		i++;
	}
	while (og[k])
	{
		ret[i] = og[k];
		i++;
		k++;
	}
	ret[i] = 0;
	return (ret);
}

void	expand_exit_code(t_tokens **tokens, t_env *ms, int si, char **tmp)
{
	char	*strerr;

	strerr = ft_itoa(ms->last_exit_code);
	if (!strerr)
		exit_clean(ms, MALLOC_ERROR);
	free((*tokens)->token);
	(*tokens)->token = expand_token(*tmp, strerr, si, si + 2);
	free(*tmp);
	free(strerr);
	if (!(*tokens)->token)
		exit_clean(ms, MALLOC_ERROR);
}

void	expand_zero(t_tokens **tokens, t_env *ms, int si, char **tmp)
{
	char	*param;

	param = ft_strdup(ms->argv[0]);
	if (!param)
		exit_clean(ms, MALLOC_ERROR);
	free((*tokens)->token);
	(*tokens)->token = expand_token(*tmp, param, si, si + 2);
	free(*tmp);
	free(param);
	if (!(*tokens)->token)
		exit_clean(ms, MALLOC_ERROR);
}

void	expand_digit_to_empty(t_tokens **tokens, t_env *ms, int si, char **tmp)
{
	free((*tokens)->token);
	(*tokens)->token = expand_token(*tmp, "", si, si + 2);
	free(*tmp);
	if (!(*tokens)->token)
		exit_clean(ms, MALLOC_ERROR);
}

bool	is_terminating_character(char c)
{
	return (c == ' ' || c == '\"' || c == '\'' || c == '\t' || c == '\n'
		|| c == '|' || c == '<' || c == '>' || c == 0);
}
