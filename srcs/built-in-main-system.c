/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in-main-system.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:57:13 by elopin            #+#    #+#             */
/*   Updated: 2025/05/06 16:46:39 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
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

bool	ft_echo(t_tokens *cmd, t_tokens *t, t_env *ms)
{
	int	newline;

	newline = 1;
	if (!ft_strcmp(cmd->token, "echo") && expand_command(&cmd, ms))
	{
		t = cmd->next;
		if (t && !ft_strcmp(t->token, "-n"))
		{
			newline = 0;
			t = t->next;
		}
		while (t)
		{
			if (t->next)
				ft_print_echo(t->token, t->next->token, newline);
			else
				ft_print_echo(t->token, NULL, newline);
			t = t->next;
		}
		return (true);
	}
	return (false);
}

bool	ft_cd_sys(t_tokens *t, t_env *ms)
{
	char	*tmp;
	char	*str;

	str = NULL;
	tmp = print_path(0);
	if (t && t->next)
		return (free(tmp), ms->last_exit_code = 1, true);
	ms->last_exit_code = 0;
	str = ft_strjoin("OLDPWD=", tmp);
	free(tmp);
	ft_export(ms, str);
	free(str);
	if (t)
		ft_cd(t->token, ms);
	else
		ft_cd(NULL, ms);
	tmp = print_path(0);
	str = ft_strjoin("PWD=", tmp);
	free(tmp);
	return (ft_export(ms, str), free(str), true);
}

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
			if (!t)
				return (true);
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

bool	found_builtin(t_tokens *cmd, t_env *ms)
{
	t_tokens	*t;
	char		*tmp;

	tmp = NULL;
	if (cmd)
		t = cmd->next;
	else
		return (false);
	if (ft_echo(cmd, t, ms))
		return (ms->last_exit_code = 0, true);
	if (!ft_strcmp(cmd->token, "cd") && expand_command(&cmd, ms))
		return (ft_cd_sys(t, ms), true);
	if (!ft_strcmp(cmd->token, "pwd") && expand_command(&cmd, ms))
		return (tmp = print_path(1), free(tmp), ms->last_exit_code = 0, true);
	if (!ft_strcmp(cmd->token, "env") && expand_command(&cmd, ms))
		return (ft_env(ms), ms->last_exit_code = 0, true);
	if (!ft_strcmp(cmd->token, "unset") && expand_command(&cmd, ms))
		return (ft_export_extra(t, ms, 1), true);
	if (!ft_strcmp(cmd->token, "exit"))
		return (ft_exit(t, ms), true);
	if (!ft_strcmp(cmd->token, "export") && expand_command(&cmd, ms))
		return (ft_export_extra(t, ms, 0), true);
	return (false);
}
