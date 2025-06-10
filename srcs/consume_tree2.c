/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:16:13 by elopin            #+#    #+#             */
/*   Updated: 2025/06/10 23:24:56 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// #include <stdlib.h>
#include <unistd.h>

int	handle_here_doc(t_ast *ast, t_env *ms);

volatile sig_atomic_t	g_heredoc_interrupted = 0;

void	handle_and(t_ast *ast, t_env *ms)
{
	consume_tree(ast->left, ms);
	if (!ms->last_exit_code)
		consume_tree(ast->right, ms);
}

int apply_redirections(t_ast *ast, t_env *ms)
{
	int	result;

	if (!ast)
		return (0);

	t_ast *curr = ast;

	while (curr && (curr->type == NODE_REDIR_IN
				|| curr->type == NODE_REDIR_OUT
				|| curr->type == NODE_APPEND_OUT
				|| curr->type == NODE_HERE_DOC))
	{
		if (curr->type == NODE_REDIR_IN)
		{
			if (handle_redir_in(curr, ms) == -1)
			{
				ms->last_exit_code = 1;
				return (-1);
			}
		}
		else if (curr->type == NODE_REDIR_OUT)
		{
			if (handle_redir_out(curr, ms) == -1)
			{
				ms->last_exit_code = 1;
				return (-1);
			}
		}
		else if (curr->type == NODE_APPEND_OUT)
		{
			if (handle_append_out(curr, ms) == -1)
			{
				ms->last_exit_code = 1;
				return (-1);
			}
		}
		else if (curr->type == NODE_HERE_DOC)
		{
			result = handle_here_doc(curr, ms);
			if (result == -1)
			{
				ms->last_exit_code = 1;
				return (-1);
			}
			if (result == 130)
			{
				ms->last_exit_code = 130;
				return (130);
			}
			
			if (dup2(curr->heredoc_fd, STDIN_FILENO) == -1)
			{
				close(curr->heredoc_fd);
				ms->last_exit_code = 1;
				return (-1);
			}
			close(curr->heredoc_fd);
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


void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	close(STDIN_FILENO);
	write(STDERR_FILENO, "\n", 1);
}

int	heredoc_loop(t_ast *ast, int pipe_fd[2])
{
	char	*line;
	
	while (1)
	{
		line = readline("here-doc> ");
		
		if (g_heredoc_interrupted || !line)
		{
			if (line)
				free(line);
			if (g_heredoc_interrupted)
				return (130);
			write(STDERR_FILENO,
				"warning: here-document delimited by end-of-file (wanted `",
				58);
			write(STDERR_FILENO, ast->file, ft_strlen(ast->file));
			write(STDERR_FILENO, "`)\n", 3);
			return (0);
		}
		
		if (!ft_strcmp(line, ast->file))
		{
			free(line);
			break; 
		}
		
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	
	return (0);
}

int	handle_here_doc(t_ast *ast, t_env *ms)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	int		saved_stdin;
	
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		return (perror("dup error"), -1);
	
	if (pipe(pipe_fd) == -1)
	{
		close(saved_stdin);
		return (perror("pipe error"), -1);
	}
	
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
	{
		close(saved_stdin);
		signal(SIGINT, handle_heredoc_sigint);
		signal(SIGQUIT, SIG_IGN);
		
		int result = heredoc_loop(ast, pipe_fd);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		
		exit_clean(ms, result);
	}
	else
	{
		close(pipe_fd[1]);
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, SIG_DFL);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			close(pipe_fd[0]);
			ms->last_exit_code = 130;
			return (130);
		}
		if (WIFEXITED(status))
		{
			int exit_code = WEXITSTATUS(status);
			if (exit_code == 130)
			{
				close(pipe_fd[0]);
				ms->last_exit_code = 130;
				return (130);
			}
			else if (exit_code != 0)
			{
				close(pipe_fd[0]);
				ms->last_exit_code = exit_code;
				return (exit_code);
			}
		}
		ast->heredoc_fd = pipe_fd[0];
		return (0);
	}
	return (0);
}

void	cleanup_readline_after_sigint(void)
{
	rl_done = 0;
	rl_replace_line("", 0);
	rl_on_new_line();
	// rl_redisplay();
}

void	consume_tree(t_ast *ast, t_env *ms)
{
	int	i;

	i = 0;
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
	{
		pid_t pid = fork();
		if (pid == -1)
			perror("fork error\n");
		else if (pid == 0)
		{
			i = apply_redirections(ast, ms);
			if (i == -1)
			{
				ms->last_exit_code = 1;
				exit_clean(ms, 1);
			}
			if (i == 130)
			{
				ms->last_exit_code = 130;
				exit_clean(ms, 130);
			}
			t_ast *cmd = get_final_cmd(ast);
			if (!cmd)
				exit_clean(ms, 1);
			consume_tree(cmd, ms);
			exit_clean(ms, ms->last_exit_code);
		}
		else
		{
			int	status;
			waitpid(pid, &status, 0);
			
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
	}
}

