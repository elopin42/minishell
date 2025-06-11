/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in-for-norme.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:03:11 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 19:41:53 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	ft_export_extra(t_tokens *t, t_env *ms, int i)
{
	if (i)
	{
		while (t)
		{
			ft_unset_co(t->token, ms);
			t = t->next;
		}
		return (true);
	}
	else
	{
		if (t && t->token)
		{
			if (!t || !ft_check_egal(t))
				return (ms->last_exit_code = 1, true);
			while (t)
			{
				ft_export(ms, t->token);
				t = t->next;
			}
			return (true);
		}
		else
			return (print_export(ms), ms->last_exit_code = 0, true);
	}
}

char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	result = malloc(strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
		{
			result[j] = str[i];
			j++;
		}
		i++;
	}
	result[j] = '\0';
	return (result);
}

bool	has_quotes(char *str)
{
	int	i;

	if (!str)
		return (false);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (true);
		i++;
	}
	return (false);
}

char	*reconstruct_from_parts(t_token_part *parts)
{
	char	*result;

	int (total_len) = 0;
	if (!parts)
		return (NULL);
	while (parts)
	{
		if (parts->content)
			total_len += ft_strlen(parts->content);
		parts = parts->next;
	}
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (parts)
	{
		if (parts->content)
			ft_strlcpy(result + ft_strlen(result), parts->content, total_len + 1
				- ft_strlen(result));
		parts = parts->next;
	}
	return (result);
}

bool	check_builtin(t_tokens *token, char *builtin, t_env *ms)
{
	char		*reconstructed;
	t_tokens	*cpy;

	if (!token || !builtin)
		return (false);
	if (token->token)
	{
		cpy = dup_token(token);
		if (!cpy)
			exit_clean(ms, MALLOC_ERROR);
		expand_command(&cpy, ms);
		if (!ft_strcmp(cpy->token, builtin))
			return (free_token(&cpy), true);
		else
			return (free_token(&cpy), false);
	}
	reconstructed = expand_parts(token->parts, ms);
	fd_printf(2, "%s\n", reconstructed);
	if (!reconstructed)
		return (false);
	if (!ft_strcmp(reconstructed, builtin))
		return (free(reconstructed), true);
	free(reconstructed);
	return (false);
}
