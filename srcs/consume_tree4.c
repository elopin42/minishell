/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:55:35 by elopin            #+#    #+#             */
/*   Updated: 2025/06/11 20:15:46 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_readline_after_sigint(void)
{
	rl_done = 0;
	rl_replace_line("", 0);
	rl_on_new_line();
}

static void	child_process_redirs(t_ast *ast, t_env *ms)
{
	int		ret;
	t_ast	*cmd;

	ret = apply_redirections(ast, ms);
	if (ret == -1)
		exit_clean(ms, 1);
	if (ret == 130)
		exit_clean(ms, 130);
	cmd = get_final_cmd(ast);
	if (!cmd)
		exit_clean(ms, 1);
	consume_tree(cmd, ms);
	exit_clean(ms, ms->last_exit_code);
}

static void	parent_process_wait(t_env *ms, int status)
{
	waitpid(-1, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		ms->last_exit_code = 130;
		cleanup_readline_after_sigint();
		return ;
	}
	if (WIFEXITED(status))
	{
		ms->last_exit_code = WEXITSTATUS(status);
		if (WEXITSTATUS(status) == 130)
			cleanup_readline_after_sigint();
	}
	else
		ms->last_exit_code = 1;
}

static void	handle_redirection_node(t_ast *ast, t_env *ms)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror("fork error\n");
	else if (pid == 0)
		child_process_redirs(ast, ms);
	else
		parent_process_wait(ms, 0);
}

void	consume_tree(t_ast *ast, t_env *ms)
{
	if (!ast)
		return ;
	if (ast->type == NODE_PIPE)
	{
		ms->saved_stdin = dup(STDIN_FILENO);
		handle_pipe(ast, ms);
	}
	else if (ast->type == NODE_AND)
		handle_and(ast, ms);
	else if (ast->type == NODE_CMD)
	{
		if (ast->cmd->token && ast->cmd->token[0] == 0)
			ms->last_exit_code = 127;
		else
			fork_and_execute_cmd(ast->cmd, ms);
	}
	else if (ast->type == NODE_REDIR_IN || ast->type == NODE_REDIR_OUT
		|| ast->type == NODE_APPEND_OUT || ast->type == NODE_HERE_DOC)
		handle_redirection_node(ast, ms);
}
