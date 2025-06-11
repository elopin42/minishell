/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in-function.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:53:56 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 17:52:44 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	no_find_key(char *env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (env[i] == '=')
			return (++i);
		i++;
	}
	return (-1);
}

void	ft_env(t_env *ms)
{
	int	i;

	i = -1;
	while (ms->envp[++i])
		printf("%s\n", ms->envp[i]);
}

char	*print_path(int a)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (a)
		printf("%s\n", path);
	return (path);
}

void	ft_unset_co(char *str, t_env *ms)
{
	int (i) = 0;
	int (j) = -1;
	if (!ms->envp && set_error(ms, 1))
		return ;
	while (ms->envp[++i])
	{
		if (!ft_strncmp(ms->envp[i], str, ft_strlen(str))
			&& (ms->envp[i][ft_strlen(str)] == '='
			|| ms->envp[i][ft_strlen(str)] == '\0'))
		{
			free(ms->envp[i]);
			j = i - 1;
			while (ms->envp[++j])
				ms->envp[j] = ms->envp[j + 1];
			return ;
		}
	}
}

void	ft_cd(char *path, t_env *ms)
{
	if (!path || !*path || !ft_strcmp(path, "~"))
		path = getenv("HOME");
	else if (!ft_strcmp(path, "-"))
	{
		path = getenv("OLDPWD");
		if (path)
			printf("%s\n", path);
		else
		{
			printf("cd: OLDPWD not set\n");
			ms->last_exit_code = 1;
			return ;
		}
	}
	if (!path)
	{
		ft_putstr_fd("cd: directory not found\n", 2);
		ms->last_exit_code = 1;
		return ;
	}
	if (chdir(path) != 0)
		return (ms->last_exit_code = 1, perror("cd"));
	ms->last_exit_code = 0;
}
