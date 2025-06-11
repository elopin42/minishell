/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_int_ultra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:41:47 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 23:09:17 by elopin           ###   ########.fr       */
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
	long	exit_code;

	int (i) = 0;
	if (!t)
		exit_clean(ms, ms->last_exit_code);
	if (!t->token && t->parts)
		t->token = expand_parts(t->parts, ms);
	if (t->token[0] == '\0')
		return (fd_printf(2, "exit: %s: numeric argument required\n", t->token),
			exit_clean(ms, 2));
	if (t->token[0] == '-' || t->token[0] == '+')
		i++;
	i--;
	while (t->token[++i])
		if (!ft_isdigit(t->token[i]))
			return (fd_printf(2, "exit: %s: numeric argument required\n",
					t->token), exit_clean(ms, 2));
	if (t->next)
	{
		fd_printf(2, "exit: too many arguments\n");
		ms->last_exit_code = 1;
		return ;
	}
	exit_code = ft_atoi(t->token);
	exit_clean(ms, exit_code);
}

bool	found_builtin(t_tokens *cmd, t_env *ms)
{
	t_tokens	*t;
	char		*tmp;

	if (!cmd)
		return (false);
	t = cmd->next;
	if (check_builtin(cmd, "echo", ms))
		return (ft_echo(cmd, t, ms), ms->last_exit_code = 0, true);
	if (check_builtin(cmd, "cd", ms) && expand_command(&cmd, ms))
		return (ft_cd_sys(t, ms), true);
	if (check_builtin(cmd, "pwd", ms) && expand_command(&cmd, ms))
		return (tmp = print_path(1), free(tmp), ms->last_exit_code = 0, true);
	if (check_builtin(cmd, "env", ms) && !cmd->next && expand_command(&cmd, ms))
		return (ft_env(ms), ms->last_exit_code = 0, true);
	if (check_builtin(cmd, "unset", ms) && expand_command(&cmd, ms))
		return (ft_export_extra(t, ms, 1), true);
	if (check_builtin(cmd, "exit", ms))
		return (printf("\033[1;31mexit\033[0m\n"), ft_exit(t, ms), true);
	if (check_builtin(cmd, "export", ms) && expand_command(&cmd, ms))
		return (ft_export_extra(t, ms, 0), true);
	return (false);
}

int	ft_shlvl(t_env *ms)
{
	int		b;
	int		i;
	char	*tmp;
	char	*str;

	b = 0;
	tmp = NULL;
	str = NULL;
	i = find_key(ms->envp, "SHLVL");
	if (i < 0 && ft_export(ms, "SHLVL=1"))
		return (set_error(ms, 1), 1);
	str = ft_substr(ms->envp[i], find_index(ms->envp[i], '=') + 1,
			ft_strlen(ms->envp[i]));
	if (!str && set_error(ms, 1))
		return (set_error(ms, 1), 1);
	b = ft_atoi(str) + 1;
	free(str);
	tmp = ft_itoa(b);
	if (!tmp)
		return (set_error(ms, 1), 0);
	str = ft_strjoin("SHLVL=", tmp);
	free(tmp);
	if (!str)
		return (0);
	return (ft_export(ms, str), free(str), 1);
}

void	ft_old_and_pwd(t_env *ms)
{
	char	*str;
	char	*tmp;

	str = print_path(0);
	tmp = NULL;
	tmp = ft_strjoin("PWD=", str);
	if (find_key(ms->envp, "OLDPWD") < 0)
		ft_export(ms, "OLDPWD");
	ft_export(ms, tmp);
	free(tmp);
	free(str);
	ft_shlvl(ms);
}
