/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_int_ultra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:41:47 by elopin            #+#    #+#             */
/*   Updated: 2025/06/09 17:52:48 by elopin           ###   ########.fr       */
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

/*void	ft_exit(t_tokens *t, t_env *ms)
{
	int	i;
	int	y;

	y = -1;
	i = -1;
	// printf("exit\n");
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
		while(t->token[++i])
			if (!ft_isdigit(t->token[i]))
			{
				printf("exit: : numeric argument required\n");
				exit_clean(ms, 2);
			}
		i = ft_atoi(t->token);
		if (i < 0)
			exit_clean(ms, 156);
		else
			exit_clean(ms, i);
	}
	else
		exit_clean(ms, 0);
}*/

void	ft_exit(t_tokens *t, t_env *ms)
{
	long	exit_code;
	int		i = 0;

	// Aucun argument
	if (!t)
		exit_clean(ms, ms->last_exit_code);

	// Cas vide : ""
	if (t->token[0] == '\0')
	{
		fd_printf(2, "exit: %s: numeric argument required\n", t->token);
		exit_clean(ms, 2);
	}

	// Vérifie si premier argument est numérique
	if (t->token[0] == '-' || t->token[0] == '+')
		i++;
	while (t->token[i])
	{
		if (!ft_isdigit(t->token[i]))
		{
			fd_printf(2, "exit: %s: numeric argument required\n", t->token);
			exit_clean(ms, 2);
		}
		i++;
	}

	// Si il y a plus d’un argument → erreur mais on NE QUITTE PAS
	if (t->next)
	{
		fd_printf(2, "exit: too many arguments\n");
		ms->last_exit_code = 1;
		return;
	}

	// Un seul argument numérique → quitter avec ce code
	exit_code = ft_atoi(t->token);
	exit_clean(ms, exit_code);
}

