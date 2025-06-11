/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 19:46:56 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 17:27:34 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

void	ft_trie_env(t_env *ms)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	if (!ms->envp && set_error(ms, 1))
		return ;
	while (ms->envp[i] && ms->envp[i + 1])
	{
		if (ft_strcmp(ms->envp[i], ms->envp[i + 1]) > 0)
		{
			tmp = ms->envp[i];
			ms->envp[i] = ms->envp[i + 1];
			ms->envp[i + 1] = tmp;
			i = -1;
		}
		i++;
	}
}

void	print_export(t_env *ms)
{
	t_env	*tmp;
	int		i;
	char	*str;

	tmp = NULL;
	i = 0;
	str = NULL;
	if (!ms)
		return ;
	tmp = ms;
	if (!tmp->envp && set_error(ms, 1))
		return ;
	ft_trie_env(ms);
	while (tmp->envp[i] != NULL)
	{
		if (ft_strchr(tmp->envp[i], '='))
			str = add_guillemet(tmp->envp[i]);
		else
			str = ft_strdup(tmp->envp[i]);
		if (!str && set_error(ms, 1))
			return ;
		printf("export %s\n", str);
		free(str);
		i++;
	}
}

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

bool	is_valid_export_identifier(const char *s)
{
	int	i;

	i = 0;
	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (false);
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	ft_export(t_env *ms, char *var)
{
	char	*key;

	int (h) = 0;
	if (!var || var[0] == '=')
		return (set_error(ms, 1), 1);
	if (!is_valid_export_identifier(var) || ft_strchr(var, '&'))
		return (printf("export: `%s': not a valid identifier\n", var),
			set_error(ms, 1), 1);
	if (!is_valid_identifier(var))
		return (set_error(ms, 1));
	key = ft_substr(var, 0, find_index(var, '='));
	if (!key && set_error(ms, 1))
		return (1);
	if (!ft_isalpha(key[0]))
		return (set_error(ms, 1), free(key), 1);
	h = find_key(ms->envp, key);
	free(key);
	if (h >= 0)
		update_export(ms, var, h);
	else
		creat_export(ms, var);
	return (ms->last_exit_code = 0, 1);
}
