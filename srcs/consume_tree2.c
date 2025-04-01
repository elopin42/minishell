/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:16:13 by elopin            #+#    #+#             */
/*   Updated: 2025/03/30 20:17:59 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_here_doc(t_ast *ast, t_env *ms)
{
	int		pipe_fd[2];
	char	*line;
	size_t	len;
	int		saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	line = 0;
	len = 0;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		return ;
	}
	while (1)
	{
		line = readline("here-doc> ");
		if (!line)
		{
			perror("readline error\n");
			return ;
		}
		if (!ft_strcmp(line, ast->file))
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	consume_tree(ast->left, ms);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
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
		execute_cmd(cmd, ms);
	else
	{
		wait(NULL);
		// fd_printf(2, "%d %s\n", __LINE__, __FILE__);
	}
}

void	handle_and(t_ast *ast, t_env *ms)
{
	consume_tree(ast->left, ms);
	consume_tree(ast->right, ms);
}

void	consume_tree(t_ast *ast, t_env *ms)
{
	if (ast->type == NODE_PIPE)
		handle_pipe(ast, ms);
	else if (ast->type == NODE_AND)
		handle_and(ast, ms);
	else if (ast->type == NODE_REDIR_IN)
		handle_redir_in(ast, ms);
	else if (ast->type == NODE_REDIR_OUT)
		handle_redir_out(ast, ms);
	else if (ast->type == NODE_HERE_DOC)
		handle_here_doc(ast, ms);
	else if (ast->type == NODE_APPEND_OUT)
		handle_append_out(ast, ms);
	else if (ast->type == NODE_CMD)
		fork_and_execute_cmd(ast->cmd, ms);
}
