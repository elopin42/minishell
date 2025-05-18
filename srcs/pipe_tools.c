/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:45:03 by tbeauman          #+#    #+#             */
/*   Updated: 2025/05/04 17:03:46 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	create_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	fork_or_die(pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

static void	update_last_status(int *last_status, int status, pid_t pid, pid_t last_pid)
{
	if (pid == last_pid)
	{
		if (WIFEXITED(status))
			*last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*last_status = 128 + WTERMSIG(status);
	}
}

void	wait_and_restore_stdin(t_env *ms, int input_fd, pid_t last_pid)
{
	int		status;
	int		last_status;
	pid_t	pid;

	close(input_fd);
	last_status = 0;
	pid = wait(&status);
	while (pid > 0)
	{
		update_last_status(&last_status, status, pid, last_pid);
		pid = wait(&status);
	}
	ms->last_exit_code = last_status;
	if (dup2(ms->saved_stdin, STDIN_FILENO) == -1)
	{
		perror("dup2 restore STDIN");
		exit(EXIT_FAILURE);
	}
	close(ms->saved_stdin);
	close_all_fds();
}
