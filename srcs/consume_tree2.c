/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:16:13 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 23:34:32 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

int						handle_here_doc(t_ast *ast, t_env *ms);

volatile sig_atomic_t	g_heredoc_interrupted = 0;

static int	handle_heredocs(t_ast **stack, int top, t_env *ms)
{
	int	result;

	int (last_fd) = -1;
	int (i) = -1;
	while (++i < top)
	{
		if (stack[i]->type == NODE_HERE_DOC)
		{
			result = handle_here_doc(stack[i], ms);
			if (result == -1)
				return (ms->last_exit_code = 1, -1);
			if (result == 130)
				return (ms->last_exit_code = 130, 130);
			if (last_fd != -1)
				close(last_fd);
			last_fd = stack[i]->heredoc_fd;
		}
	}
	if (last_fd != -1)
	{
		if (dup2(last_fd, STDIN_FILENO) == -1)
			return (close(last_fd), ms->last_exit_code = 1, -1);
		close(last_fd);
	}
	return (0);
}

static int	handle_other_redirs(t_ast **stack, int top, t_env *ms)
{
	int	i;

	i = top;
	while (--i >= 0)
	{
		if (stack[i]->type == NODE_REDIR_IN)
		{
			if (handle_redir_in(stack[i], ms) == -1)
				return (ms->last_exit_code = 1, -1);
		}
		else if (stack[i]->type == NODE_REDIR_OUT)
		{
			if (handle_redir_out(stack[i], ms) == -1)
				return (ms->last_exit_code = 1, -1);
		}
		else if (stack[i]->type == NODE_APPEND_OUT)
		{
			if (handle_append_out(stack[i], ms) == -1)
				return (ms->last_exit_code = 1, -1);
		}
	}
	return (0);
}

int	apply_redirections(t_ast *ast, t_env *ms)
{
	t_ast	*stack[256];
	int		top;
	int		ret;

	top = 0;
	while (ast && (ast->type == NODE_REDIR_IN || ast->type == NODE_REDIR_OUT
			|| ast->type == NODE_APPEND_OUT || ast->type == NODE_HERE_DOC))
	{
		stack[top++] = ast;
		ast = ast->right;
	}
	ret = handle_heredocs(stack, top, ms);
	if (ret != 0)
		return (ret);
	return (handle_other_redirs(stack, top, ms));
}

t_ast	*get_final_cmd(t_ast *ast)
{
	while (ast && ast->type != NODE_CMD)
		ast = ast->left;
	return (ast);
}
