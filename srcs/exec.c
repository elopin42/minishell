/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:42:38 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 21:19:28 by elopin           ###   ########.fr       */
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

static char	*get_cmd_path(t_tokens *cmd, t_env *ms)
{
	char	*path;

	path = NULL;
	if (access(cmd->token, F_OK) == 0)
	{
		if (access(cmd->token, X_OK) == 0)
		{
			path = ft_strdup(cmd->token);
			if (!path)
				exit_clean(ms, EXIT_FAILURE);
		}
		else
		{
			fd_printf(2, "%s: Permission denied\n", cmd->token);
			exit_clean(ms, 126);
		}
	}
	else
	{
		path = get_path(cmd->token, ms->envp, ms);
		if (!path)
			return (fd_printf(2, "%s: command not found\n", cmd->token),
				exit_clean(ms, 127), NULL);
	}
	return (path);
}

void	execute_cmd(t_tokens *cmd, t_env *ms)
{
	char	*path;
	char	**argscmd;
	int		code;

	if (!cmd->token || cmd->token[0] == '\0')
		exit_clean(ms, 0);
	path = get_cmd_path(cmd, ms);
	argscmd = tokens_to_array(cmd);
	if (!argscmd)
		exit_clean(ms, EXIT_FAILURE);
	if (execve(path, argscmd, ms->envp) == -1)
	{
		if (errno == ENOENT)
			perror("No such file or directory");
		else if (errno == EACCES || errno == EISDIR)
			perror("Permission denied");
		else
			perror("Misc error");
		if (errno == EACCES || errno == EISDIR)
			code = 126;
		else
			code = 1;
		exit_clean(ms, code);
	}
}

void	wait_for_child(pid_t pid, t_env *ms)
{
	int	status;
	int	sig;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ms->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		ms->last_exit_code = 128 + sig;
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(1, "Quit (core dumped)\n", 20);
	}
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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		expand_command(&cmd, ms);
		while (cmd && (!cmd->token || cmd->token[0] == '\0'))
			cmd = cmd->next;
		if (!cmd)
			exit_clean(ms, 0);
		execute_cmd(cmd, ms);
		return ;
	}
	wait_for_child(pid, ms);
	if (ms->ast && ms->ast->type == NODE_PIPE)
	{
		dup2(ms->saved_stdin, STDIN_FILENO);
		close(ms->saved_stdin);
	}
}
