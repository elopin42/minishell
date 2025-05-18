/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:42:38 by elopin            #+#    #+#             */
/*   Updated: 2025/05/06 17:27:33 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

char	**tokens_to_array(t_tokens *cmd)
{
	char		**ret;
	int			i;
	t_tokens	*head;

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
	{
		path = ft_strdup(cmd->token);
		if (!path)
			exit_clean(ms, EXIT_FAILURE);
	}
	else
	{
		path = get_path(cmd->token, ms->envp, ms);
		if (!path)
			exit_clean(ms,CMD_NOT_FOUND_ERROR);
	}
	argscmd = tokens_to_array(cmd);
	if (!argscmd)
		exit_clean(ms,EXIT_FAILURE);
	if (execve(path, argscmd, ms->envp) == -1)
	{
		if (errno == ENOENT)
			exit_clean(ms, CMD_NOT_FOUND_ERROR);
		else if (errno == EACCES || errno == EISDIR)
			exit_clean(ms, 126);
		else
			exit_clean(ms, 1);
	}
	return (true);
}

void	wait_for_child(pid_t pid, t_env *ms)
{
	int		status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ms->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ms->last_exit_code = 128 + WTERMSIG(status);
}

void	fork_and_execute_cmd(t_tokens *cmd, t_env *ms)
{
	pid_t	pid;

	if (found_builtin(cmd, ms))
		return ;
	fork_or_die(&pid);
	ms->pididi = pid;
	if (pid == 0)
	{
		expand_command(&cmd, ms);
		execute_cmd(cmd, ms);
	}
	else
	{
		wait_for_child(pid, ms);
		if (ms->ast && ms->ast->type == NODE_PIPE)
		{
			dup2(ms->saved_stdin, STDIN_FILENO);
			close(ms->saved_stdin);
		}
	}
}
