/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:16:13 by elopin            #+#    #+#             */
/*   Updated: 2025/05/05 17:57:13 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_here_doc(t_ast *ast, t_env *ms)
{
	int		pipe_fd[2];
	char	*line;
	// int		saved_stdin;
	(void)ms;

	// saved_stdin = dup(STDIN_FILENO);
	line = 0;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		return (-1);
	}
	while (1)
	{
		line = readline("here-doc> ");
		if (!line)
		{
			perror("readline error\n");
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			return (-1);
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
	ast->heredoc_fd = pipe_fd[0];
	return (0);
	// dup2(pipe_fd[0], STDIN_FILENO);
	// close(pipe_fd[0]);
	// consume_tree(ast->left, ms);
	// dup2(saved_stdin, STDIN_FILENO);
	// close(saved_stdin);
}

void	handle_and(t_ast *ast, t_env *ms)
{
	consume_tree(ast->left, ms);
	if (!ms->last_exit_code)
		consume_tree(ast->right, ms);
}

int apply_redirections(t_ast *ast, t_env *ms)
{
	if (!ast)
		return 0;

	t_ast *curr = ast;

	while (curr && (curr->type == NODE_REDIR_IN
				|| curr->type == NODE_REDIR_OUT
				|| curr->type == NODE_APPEND_OUT
				|| curr->type == NODE_HERE_DOC))
	{
		if (curr->type == NODE_REDIR_IN && handle_redir_in(curr, ms) == -1)
			return (-1);
		else if (curr->type == NODE_REDIR_OUT && handle_redir_out(curr, ms) == -1)
			return (-1);
		else if (curr->type == NODE_APPEND_OUT && handle_append_out(curr, ms) == -1)
			return (-1);
		else if (curr->type == NODE_HERE_DOC)
		{
			if (handle_here_doc(curr, ms) == -1)
				return (-1);
			if (dup2(curr->heredoc_fd, STDIN_FILENO) == -1)
				return (-1);
			close(curr->heredoc_fd);
				return (0);
		}
		curr = curr->right;
	}
	return (0);
}


t_ast	*get_final_cmd(t_ast *ast)
{
	while (ast && ast->type != NODE_CMD)
		ast = ast->left;
	return (ast);
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
		fork_and_execute_cmd(ast->cmd, ms);
	else if (ast->type == NODE_REDIR_IN || ast->type == NODE_REDIR_OUT
			|| ast->type == NODE_APPEND_OUT || ast->type == NODE_HERE_DOC)
	{
		pid_t pid = fork();
		if (pid == -1)
			perror("fork error\n");
		else if (pid == 0)
		{
			if (apply_redirections(ast, ms) == -1)
				exit_clean(ms, 1);
			t_ast *cmd = get_final_cmd(ast);
			if (!cmd)
				exit_clean(ms, 1);
			consume_tree(cmd, ms);
			exit(EXIT_SUCCESS);
		}
		else
		{
			int	status;
			waitpid(pid, &status, 0);
			
			if (WIFEXITED(status))
				ms->last_exit_code = WEXITSTATUS(status);
			else
				ms->last_exit_code = 1;
		}
	}
}
