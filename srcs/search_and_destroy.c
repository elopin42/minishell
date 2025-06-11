/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_and_destroy.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:19:32 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 22:29:12 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_special_chars(t_tokens **tokens, t_env *ms, int si)
{
	char	*tmp;
	char	next;

	tmp = ft_strdup((*tokens)->token);
	if (!tmp)
		return (0);
	next = (*tokens)->token[si + 1];
	if (next == '?')
	{
		expand_exit_code(tokens, ms, si, &tmp);
		return (1);
	}
	if (next >= '0' && next <= '9')
	{
		if (next == '0')
			expand_zero(tokens, ms, si, &tmp);
		else
			expand_digit_to_empty(tokens, ms, si, &tmp);
		return (1);
	}
	free(tmp);
	return (0);
}

static char	*get_env_value(t_env *ms, char *name)
{
	int		i;
	char	*var;

	i = 0;
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], name, ft_strlen(name)) == 0
			&& ms->envp[i][ft_strlen(name)] == '=')
			break ;
		i++;
	}
	if (!ms->envp[i])
		var = ft_strdup("");
	else
		var = ft_substr(ms->envp[i], ft_strlen(name) + 1,
				ft_strlen(ms->envp[i]));
	return (var);
}

static void	expand_variable(t_tokens **tokens, t_env *ms, int si)
{
	char	*tmp;
	char	*name;
	char	*var;
	int		stop;

	tmp = ft_strdup((*tokens)->token);
	if (!tmp)
		exit_clean(ms, MALLOC_ERROR);
	stop = si + 1;
	while ((*tokens)->token[stop] && (ft_isalnum((*tokens)->token[stop])
			|| (*tokens)->token[stop] == '_'))
		stop++;
	name = ft_substr(tmp, si + 1, stop - (si + 1));
	if (!name)
		return (free(tmp), exit_clean(ms, MALLOC_ERROR));
	var = get_env_value(ms, name);
	if (!var)
		return (free(name), free(tmp), exit_clean(ms, MALLOC_ERROR));
	free((*tokens)->token);
	(*tokens)->token = expand_token(tmp, var, si, stop);
	free(tmp);
	free(var);
	free(name);
	if (!(*tokens)->token)
		exit_clean(ms, MALLOC_ERROR);
}

void	search_and_destroy(t_tokens **tokens, t_env *ms)
{
	int		si;
	char	next;

	if (!*tokens || !(*tokens)->token)
		return ;
	si = ft_seek_index((*tokens)->token, '$');
	if (si == -1)
		return ;
	next = (*tokens)->token[si + 1];
	if (!(next == '?' || (next >= '0' && next <= '9') || next == '_'
			|| ft_isalpha(next)))
		return ;
	if (handle_special_chars(tokens, ms, si))
	{
		if (ft_seek_index((*tokens)->token, '$') != -1)
			search_and_destroy(tokens, ms);
		return ;
	}
	expand_variable(tokens, ms, si);
	if (ft_seek_index((*tokens)->token, '$') != -1)
		search_and_destroy(tokens, ms);
}
