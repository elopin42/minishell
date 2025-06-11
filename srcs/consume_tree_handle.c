/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree_handle.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:47:20 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 19:52:40 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	heredoc_child(t_ast *ast, int *pipe_fd, t_env *ms)
{
	int	result;

	close(pipe_fd[0]);
	signal(SIGINT, handle_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	result = heredoc_loop(ast, pipe_fd);
	close(pipe_fd[1]);
	exit_clean(ms, result);
	return (0);
}

static int	heredoc_parent(pid_t pid, int *pipe_fd, int saved_stdin, t_env *ms)
{
	int	status;
	int	exit_code;

	close(pipe_fd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, SIG_DFL);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (close(pipe_fd[0]), ms->last_exit_code = 130, 130);
	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		if (exit_code != 0)
			return (close(pipe_fd[0]), ms->last_exit_code = exit_code,
				exit_code);
	}
	return (0);
}

int	handle_here_doc(t_ast *ast, t_env *ms)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		return (perror("dup error"), -1);
	if (pipe(pipe_fd) == -1)
		return (close(saved_stdin), perror("pipe error"), -1);
	g_heredoc_interrupted = 0;
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		close(saved_stdin);
		return (perror("fork error"), -1);
	}
	if (pid == 0)
		heredoc_child(ast, pipe_fd, ms);
	if (heredoc_parent(pid, pipe_fd, saved_stdin, ms) != 0)
		return (130);
	ast->heredoc_fd = pipe_fd[0];
	return (0);
}
