/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:42:38 by elopin            #+#    #+#             */
/*   Updated: 2025/03/16 17:57:17 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*join_path(char *cmd, char **paths, t_env *ms)
{
	char	*path;
	char	*tmp;
	int		i;

	i = 0;
	path = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
		{
			fd_printf(2, "Malloc error %d %s\n", __LINE__, __FILE__);
			exit_clean(ms, EXIT_FAILURE);
		}
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		tmp = 0;
		if (!path)
		{
			fd_printf(2, "Malloc error %d %s\n", __LINE__, __FILE__);
			exit_clean(ms, EXIT_FAILURE);
		}
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		path = 0;
		i++;
	}
	free_tab(paths);
	return (0);
}

char	*get_path(char *cmd, char **envp, t_env *ms)
{
	char	**paths;
	int		i;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	if (!envp[i])
	{
		fd_printf(2, "Command not found %d %s\n", __LINE__, __FILE__);
		exit_clean(ms, EXIT_FAILURE);
	}
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
	{
		fd_printf(2, "Malloc error %d %s\n", __LINE__, __FILE__);
		exit_clean(ms, EXIT_FAILURE);
	}
	return(join_path(cmd, paths, ms));
}

void	execute(t_env *ms)
{
	char	*path;

	if (access(ms->tokens[0], F_OK) == 0)
		path = ft_strdup(ms->tokens[0]);
	else
		path = get_path(ms->tokens[0], ms->envp, ms);
	if (!path)
		fd_printf(2, "Command not found %d %s\n", __LINE__, __FILE__);
	if (execve(path, ms->tokens, ms->envp) == -1)
	{
		free(path);
		exit_clean(ms, EXIT_FAILURE);
	}
}
