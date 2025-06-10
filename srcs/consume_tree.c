/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:01:50 by elopin            #+#    #+#             */
/*   Updated: 2025/06/10 14:22:37 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_append_out(t_ast *ast, t_env *ms)
{
	int	out_fd;
	// int	saved_stdout;

	(void)ms;
	// saved_stdout = dup(STDOUT_FILENO);
	out_fd = open(ast->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (out_fd == -1)
	{
		perror("OPEN ERROR\n");
		return (-1);
	}
	if (dup2(out_fd, STDOUT_FILENO) == -1)
	{
		close(out_fd);
		perror("dup2 error\n");
		return (-1);
	}
	close(out_fd);
	return (0);
}

int	handle_redir_out(t_ast *ast, t_env *ms)
{
	int	out_fd;
(void)ms;

	out_fd = open(ast->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (out_fd == -1)
	{
		perror("OPEN ERROR\n");
		return (-1);
	}
	if (dup2(out_fd, STDOUT_FILENO) == -1)
	{
		close(out_fd);
		perror("dup2 error\n");
		return (-1);
	}
	close(out_fd);
	return (0);
}

int	handle_redir_in(t_ast *ast, t_env *ms)
{
	int	in_fd;
	// int	saved_stdin;
	(void)ms;

	// saved_stdin = dup(STDIN_FILENO);
	in_fd = open(ast->file, O_RDONLY);
	if (in_fd == -1)
	{
		perror("OPEN ERROR\n");
		return (-1);
	}
	if (dup2(in_fd, STDIN_FILENO) == -1)
	{
		close(in_fd);
		perror("dup2 error\n");
		return (-1);
	}
	close(in_fd);
	return (0);
}
