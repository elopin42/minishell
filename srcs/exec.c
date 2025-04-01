/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:42:38 by elopin            #+#    #+#             */
/*   Updated: 2025/03/30 20:17:58 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

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
	return (join_path(cmd, paths, ms));
}

char	**tokens_to_array(t_tokens *cmd)
{
	char	**ret;
	int		i;

	i = 0;
	ret = (char **)malloc((1 + ft_lst_tokens_size(cmd)) * sizeof(char *));
	if (!ret)
		return (NULL);
	while (cmd)
	{
		ret[i] = ft_strdup(cmd->token);
		if (!ret[i])
		{
			while (--i)
				free(ret[i]);
			free(ret);
			return (NULL);
		}
		i++;
		cmd = cmd->next;
	}
	ret[i] = NULL;
	return (ret);
}

bool	execute_cmd(t_tokens *cmd, t_env *ms)
{
	char	*path;
	char	**argscmd;

	if (access(cmd->token, F_OK) == 0)
		path = ft_strdup(cmd->token);
	else
		path = get_path(cmd->token, ms->envp, ms);
	//fd_printf(1, "%s\n", path);
	if (!path)
		fd_printf(2, "Command not found %d %s\n", __LINE__, __FILE__);
	argscmd = tokens_to_array(cmd);
	if (!argscmd)
	{
		fd_printf(1, "Malloc error\n");
		exit(EXIT_FAILURE);
	}
	print_tab(argscmd);
	if (execve(path, argscmd, ms->envp) == -1)
	{
		free(path);
		fd_printf(2, "\n%d %s\n", __FILE__, __LINE__);
		exit_clean(ms, EXIT_FAILURE);
	}
	return (true);
}














