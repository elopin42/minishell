/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:01:50 by elopin            #+#    #+#             */
/*   Updated: 2025/05/04 16:40:23 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	pipe_child_process(t_ast *current, int input_fd, int *pipe_fd,
		t_env *ms)
{
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 input_fd");
			exit(EXIT_FAILURE);
		}
		close(input_fd);
	}
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 pipe_fd[1]");
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	if (!current->left)
		exit(EXIT_FAILURE);
	close_all_fds();
	consume_tree(current->left, ms);
	cleanup(ms);
	exit(EXIT_SUCCESS);
}

static void	pipe_last_child_process(t_ast *current, int input_fd,
		t_env *ms)
{
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 last command input_fd");
			exit(EXIT_FAILURE);
		}
		close(input_fd);
	}
	close_all_fds();
	consume_tree(current, ms);
	cleanup(ms);
	exit(EXIT_SUCCESS);
}

static int	process_pipes_loop(t_ast **current, t_env *ms)
{
	int		input_fd;
	int		pipe_fd[2];
	pid_t	pid;

	input_fd = STDIN_FILENO;
	while (*current && (*current)->type == NODE_PIPE)
	{
		create_pipe(pipe_fd);
		fork_or_die(&pid);
		if (pid == 0)
			pipe_child_process(*current, input_fd, pipe_fd, ms);
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		close(pipe_fd[1]);
		input_fd = pipe_fd[0];
		*current = (*current)->right;
	}
	return (input_fd);
}

static void	launch_last_process(t_ast *curr, int input_fd, t_env *ms)
{
	pid_t	pid;

	if (!curr)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		pipe_last_child_process(curr, input_fd, ms);
	wait_and_restore_stdin(ms, input_fd, pid);
}

void	handle_pipe(t_ast *ast, t_env *ms)
{
	int		input_fd;
	t_ast	*current;

	current = ast;
	if (!current)
		exit(EXIT_FAILURE);
	input_fd = process_pipes_loop(&current, ms);
	launch_last_process(current, input_fd, ms);
}
