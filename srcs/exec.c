/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:42:38 by elopin            #+#    #+#             */
/*   Updated: 2025/03/18 17:38:31 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

char	*join_path(char *cmd, char **paths, t_env *ms)
{
	char	*path;
	char	*tmp;
	int		i;

	i = 0;
	path = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
		{
			fd_printf(2, "Malloc error %d %s\n", __LINE__, __FILE__);
			exit_clean(ms, EXIT_FAILURE);
		}
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		tmp = 0;
		if (!path)
		{
			fd_printf(2, "Malloc error %d %s\n", __LINE__, __FILE__);
			exit_clean(ms, EXIT_FAILURE);
		}
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		path = 0;
		i++;
	}
	free_tab(paths);
	return (0);
}

char	*get_path(char *cmd, char **envp, t_env *ms)
{
	char	**paths;
	int		i;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	if (!envp[i])
	{
		fd_printf(2, "Command not found %d %s\n", __LINE__, __FILE__);
		exit_clean(ms, EXIT_FAILURE);
	}
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
	{
		fd_printf(2, "Malloc error %d %s\n", __LINE__, __FILE__);
		exit_clean(ms, EXIT_FAILURE);
	}
	return (join_path(cmd, paths, ms));
}

char	**tokens_to_array(t_tokens *cmd)
{
	char	**ret;
	int		i;

	i = 0;
	ret = (char **)malloc((1 + ft_lst_tokens_size(cmd)) * sizeof(char *));
	if (!ret)
		return (NULL);
	while (cmd)
	{
		ret[i] = ft_strdup(cmd->token);
		if (!ret[i])
		{
			while (--i)
				free(ret[i]);
			free(ret);
			return (NULL);
		}
		i++;
		cmd = cmd->next;
	}
	ret[i] = NULL;
	return (ret);
}

void	execute_cmd(t_tokens *cmd, t_env *ms)
{
	char	*path;
	char	**argscmd;

	if (access(cmd->token, F_OK) == 0)
		path = ft_strdup(cmd->token);
	else
		path = get_path(cmd->token, ms->envp, ms);
	// fd_printf(1, "%s\n", path);
	if (!path)
		fd_printf(2, "Command not found %d %s\n", __LINE__, __FILE__);
	argscmd = tokens_to_array(cmd);
	if (!argscmd)
	{
		fd_printf(1, "Malloc error\n");
		exit(EXIT_FAILURE);
	}
	// print_tab(argscmd);
	if (execve(path, argscmd, ms->envp) == -1)
	{
		free(path);
		fd_printf(2, "\n%d %s\n", __FILE__, __LINE__);
		exit_clean(ms, EXIT_FAILURE);
	}
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
		exit(EXIT_FAILURE) ;
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
void	handle_and(t_ast *ast, t_env *ms)
{
	consume_tree(ast->left, ms);
	consume_tree(ast->right, ms);
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

void	handle_redir_in(t_ast *ast, t_env *ms)
{
	int		in_fd;
	int		saved_stdin;

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

void	handle_redir_out(t_ast *ast, t_env *ms)
{
	int		out_fd;
	int		saved_stdout;

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

void	handle_append_out(t_ast *ast, t_env *ms)
{
	int		out_fd;
	int		saved_stdout;

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