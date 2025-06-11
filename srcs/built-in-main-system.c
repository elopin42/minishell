/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in-main-system.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:57:13 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 19:04:06 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdbool.h>
#include <stdio.h>

void	ft_print_echo(char *s1, char *s2, int i)
{
	if (i)
	{
		if (s2)
			printf("%s ", s1);
		else
			printf("%s\n", s1);
	}
	else if (!i)
	{
		if (s2)
			printf("%s ", s1);
		else
			printf("%s", s1);
	}
}

bool	is_valid_n_flag(const char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (false);
	i = 2;
	while (s[i])
	{
		if (s[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

bool	ft_echo(t_tokens *cmd, t_tokens *t, t_env *ms)
{
	int (newline) = 1;
	if (!check_builtin(cmd, "echo", ms) || !expand_command(&cmd, ms))
		return (false);
	t = cmd->next;
	while (t && t->token && is_valid_n_flag(t->token))
	{
		newline = 0;
		t = t->next;
	}
	if (!t && newline)
		return (fd_printf(1, "\n"), true);
	else if (!t)
		return (true);
	while (t)
	{
		if (t->token)
		{
			if (t->next && t->next->token)
				ft_print_echo(t->token, t->next->token, newline);
			else
				ft_print_echo(t->token, NULL, newline);
		}
		t = t->next;
	}
	return (true);
}

bool	ft_cd_sys(t_tokens *t, t_env *ms)
{
	char	*tmp;
	char	*str;

	str = NULL;
	tmp = print_path(0);
	if (t && t->next)
		return (free(tmp), ms->last_exit_code = 1, true);
	str = ft_strjoin("OLDPWD=", tmp);
	free(tmp);
	ft_export(ms, str);
	free(str);
	if (t)
		ft_cd(t->token, ms);
	else
		ft_cd(NULL, ms);
	if (ms->last_exit_code == 0)
	{
		tmp = print_path(0);
		str = ft_strjoin("PWD=", tmp);
		free(tmp);
		ft_export(ms, str);
		free(str);
	}
	return (true);
}

bool	ft_check_egal(t_tokens *t)
{
	while (t)
	{
		if (t->token[0] == '=')
		{
			printf("export: `%s': not a valid identifier\n", t->token);
			return (false);
		}
		t = t->next;
	}
	return (true);
}
