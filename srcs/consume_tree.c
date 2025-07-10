/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:01:50 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 23:34:05 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_append_out(t_ast *ast, t_env *ms)
{
	int	out_fd;

	(void)ms;
	out_fd = open(ast->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (out_fd == -1)
	{
		return (-1);
	}
	if (dup2(out_fd, STDOUT_FILENO) == -1)
	{
		close(out_fd);
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
		return (-1);
	}
	if (dup2(out_fd, STDOUT_FILENO) == -1)
	{
		close(out_fd);
		return (-1);
	}
	close(out_fd);
	return (0);
}

int	handle_redir_in(t_ast *ast, t_env *ms)
{
	int	in_fd;

	(void)ms;
	in_fd = open(ast->file, O_RDONLY);
	if (in_fd == -1)
	{
		return (-1);
	}
	if (dup2(in_fd, STDIN_FILENO) == -1)
	{
		close(in_fd);
		return (-1);
	}
	close(in_fd);
	return (0);
}

void	handle_and(t_ast *ast, t_env *ms)
{
	consume_tree(ast->left, ms);
	if (!ms->last_exit_code)
		consume_tree(ast->right, ms);
}

void	handle_or(t_ast *ast, t_env *ms)
{
	if (!ast || !ast->left || !ast->right)
		return ;
	consume_tree(ast->left, ms);
	if (ms->last_exit_code != 0)
		consume_tree(ast->right, ms);
}
