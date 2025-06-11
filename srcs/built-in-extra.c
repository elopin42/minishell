/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in-extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:55:22 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 17:10:31 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_index(char *str, char c)
{
	int	i;

	i = -1;
	if (!str)
		return (-1);
	while (str[++i])
		if (str[i] == c)
			return (i);
	return (i);
}

int	find_key(char **env, char *key)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	if (!env || key_len <= 0)
		return (-1);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && (env[i][key_len] == '='
			|| env[i][key_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

void	update_export(t_env *ms, char *var, int h)
{
	char	*new_entry;

	new_entry = NULL;
	if (h == -1 && set_error(ms, 1))
		return ;
	new_entry = ft_strdup(var);
	if (!new_entry && set_error(ms, 1))
		return ;
	if (ms->envp[h] && set_error(ms, 1))
		free(ms->envp[h]);
	ms->envp[h] = new_entry;
}

void	creat_export(t_env *ms, char *var)
{
	int		size_env;
	char	**new_env;
	int		i;

	i = -1;
	size_env = 0;
	if (!ms->envp && set_error(ms, 1))
		return ;
	while (ms->envp[size_env])
		size_env++;
	new_env = ft_calloc((size_env + 2), sizeof(char *));
	if (!new_env && set_error(ms, 1))
		return ;
	while (++i < size_env)
		new_env[i] = ms->envp[i];
	new_env[size_env] = ft_strdup(var);
	new_env[size_env + 1] = NULL;
	free(ms->envp);
	ms->envp = new_env;
}

char	*add_guillemet(char *envp)
{
	char	*str;
	char	*tmp;

	if (!envp)
		return (NULL);
	str = ft_substr(envp, 0, no_find_key(envp));
	if (!str)
		return (NULL);
	if (envp[ft_strlen(str)] != '\"')
		tmp = ft_strjoin(str, "\"");
	else
		tmp = ft_substr(str, 0, ft_strlen(str));
	free(str);
	if (!tmp)
		return (NULL);
	return (add_quotes_utils(tmp, envp));
}
