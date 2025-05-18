/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_int_ultra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:41:47 by elopin            #+#    #+#             */
/*   Updated: 2025/05/06 17:07:39 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*add_quotes_utils(char *str, char *envp)
{
	char	*tmp;
	char	*tmps;

	tmps = ft_substr(envp, no_find_key(envp), ft_strlen(envp));
	if (!tmps)
		return (free(str), NULL);
	tmp = ft_strjoin(str, tmps);
	free(str);
	free(tmps);
	if (!tmp)
		return (NULL);
	if (tmp[ft_strlen(tmp) - 1] != '\"')
	{
		str = ft_strjoin(tmp, "\"");
		free(tmp);
		if (!str)
			return (NULL);
		return (str);
	}
	return (tmp);
}

void	ft_exit(t_tokens *t, t_env *ms)
{
	int	i;

	i = -1;
	printf("exit\n");
	if (t && !t->next)
	{
		while (t->token[++i])
		{
			if (ft_isalpha(t->token[i]))
			{
				set_error(ms, 2);
				exit_clean(ms, 2);
			}
		}
		i = ft_atoi(t->token);
		if (i < 0)
			exit_clean(ms, 156);
		else
			exit_clean(ms, i);
	}
	else
		exit_clean(ms, 1);
}
