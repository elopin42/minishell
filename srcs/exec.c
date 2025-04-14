/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:42:38 by elopin            #+#    #+#             */
/*   Updated: 2025/04/12 02:35:46 by tbeauman         ###   ########.fr       */
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
	t_tokens *head;

	head = cmd;
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
	cmd = head;
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
	if (execve(path, argscmd, ms->envp) == -1)
	{
		free(path);
		fd_printf(2, "\n%d %s\n", __FILE__, __LINE__);
		exit_clean(ms, EXIT_FAILURE);
	}
	return (true);
}

void	fork_and_execute_cmd(t_tokens *cmd, t_env *ms)
{
	pid_t	pid;

	if (found_builtin(cmd, ms))
		return ;
	pid = fork();
	ms->pididi = pid;
	if (pid == -1)
	{
		perror("fork");
		fd_printf(2, "\n%d %s\n", __FILE__, __LINE__);
		exit_clean(ms, EXIT_FAILURE);
	}
	if (pid == 0)
	{		
		expand_command(&cmd, ms);
		execute_cmd(cmd, ms);
	}
	else
	{
		wait(NULL);
		if (ms->ast->type == NODE_PIPE)
		{
			dup2(ms->saved_stdin, STDIN_FILENO);
			close(ms->saved_stdin);
		}
	}
}

