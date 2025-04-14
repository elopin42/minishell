/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:16:13 by elopin            #+#    #+#             */
/*   Updated: 2025/04/11 17:13:40 by tbeauman         ###   ########.fr       */
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

void	handle_and(t_ast *ast, t_env *ms)
{
	consume_tree(ast->left, ms);
	consume_tree(ast->right, ms);
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
