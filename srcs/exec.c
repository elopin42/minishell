/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:42:38 by elopin            #+#    #+#             */
/*   Updated: 2025/06/10 18:11:38 by elopin           ###   ########.fr       */
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

// void	execute_cmd(t_tokens *cmd, t_env *ms)
// {
// 	char	*path;
// 	char	**argscmd;

// 	if (access(cmd->token, F_OK) == 0)
// 	{
// 		path = ft_strdup(cmd->token);
// 		if (!path)
// 			exit_clean(ms, EXIT_FAILURE);
// 	}
// 	else
// 	{
// 		path = get_path(cmd->token, ms->envp, ms);
// 		if (!path)
// 		{
// 			perror("cmd not found\n");
// 			exit_clean(ms, CMD_NOT_FOUND_ERROR);
// 		}
// 	}
// 	argscmd = tokens_to_array(cmd);
// 	if (!argscmd)
// 		exit_clean(ms, EXIT_FAILURE);
// 	if (execve(path, argscmd, ms->envp) == -1)
// 	{
// 		if (errno == ENOENT)
// 		{
// 			perror("cmd not found");
// 			exit_clean(ms, CMD_NOT_FOUND_ERROR);
// 		}
// 		else if (errno == EACCES || errno == EISDIR)
// 		{
// 			perror("Access denied or is a directory");
// 			exit_clean(ms, 126);
// 		}
// 		else
// 		{
// 			perror("Misc error");
// 			exit_clean(ms, 1);
// 		}
// 	}
// }

void    execute_cmd(t_tokens *cmd, t_env *ms)
{
    char    *path;
    char    **argscmd;
    
	if (!cmd->token || cmd->token[0] == '\0')
		exit_clean(ms, 0);

    // if (strchr(cmd->token, '/'))
    // {
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
            // exit_clean(ms, 127);
    // }
    // else
    {
        path = get_path(cmd->token, ms->envp, ms);
        if (!path)
        {
			fd_printf(2, "%s: command not found\n", cmd->token);
            exit_clean(ms, 127);
        }
    }
    
    argscmd = tokens_to_array(cmd);
    if (!argscmd)
        exit_clean(ms, EXIT_FAILURE);
    
    if (execve(path, argscmd, ms->envp) == -1)
    {
        if (errno == ENOENT)
        {
            perror("No such file or directory");
            exit_clean(ms, 127);
        }
        else if (errno == EACCES || errno == EISDIR)
        {
            perror("Permission denied");
            exit_clean(ms, 126);
        }
        else
        {
            perror("Misc error");
            exit_clean(ms, 1);
        }
    }
}

void	wait_for_child(pid_t pid, t_env *ms)
{
	int		status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ms->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
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
		signal(SIGINT, SIG_DFL);   // Ctrl+C
		signal(SIGQUIT, SIG_DFL);
	// fd_printf(2, "%d %s: cmd: %s\n", __LINE__, __FILE__, cmd);
		expand_command(&cmd, ms);
	// fd_printf(2, "%d %s\n", __LINE__, __FILE__);
		while (cmd && (!cmd->token || cmd->token[0] == '\0'))
			cmd = cmd->next;
		if (!cmd)
			exit_clean(ms, 0);
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
