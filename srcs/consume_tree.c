/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:01:50 by elopin            #+#    #+#             */
/*   Updated: 2025/03/30 20:03:24 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_append_out(t_ast *ast, t_env *ms)
{
	int	out_fd;
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	out_fd = open(ast->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (out_fd == -1)
	{
		perror("OPEN ERROR\n");
		return ;
	}
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
	consume_tree(ast->left, ms);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
}

void	handle_redir_out(t_ast *ast, t_env *ms)
{
	int	out_fd;
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	out_fd = open(ast->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (out_fd == -1)
	{
		perror("OPEN ERROR\n");
		return ;
	}
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
	consume_tree(ast->left, ms);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
}

void	handle_redir_in(t_ast *ast, t_env *ms)
{
	int	in_fd;
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	in_fd = open(ast->file, O_RDONLY);
	if (in_fd == -1)
	{
		perror("OPEN ERROR\n");
		return ;
	}
	dup2(in_fd, STDIN_FILENO);
	close(in_fd);
	consume_tree(ast->left, ms);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

void	handle_pipe(t_ast *ast, t_env *ms)
{
	int		fd[2];
	int		saved_stdin;
	pid_t	pid;

	saved_stdin = dup(STDIN_FILENO);
	if (pipe(fd) == -1)
	{
		perror("pipe error\n");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork\n");
		exit(EXIT_FAILURE);
	}
	if (pid == CHILD)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		consume_tree(ast->left, ms);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, NULL, CHILD);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		consume_tree(ast->right, ms);
	}
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}
