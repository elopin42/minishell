/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree_norme.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:12:33 by elopin            #+#    #+#             */
/*   Updated: 2025/06/12 21:53:46 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		if (g_heredoc_interrupted)
			return (130);
		if (!line)
		{
			write(STDERR_FILENO,
				"warning: here-document delimited by end-of-file (wanted `",
				58);
			write(STDERR_FILENO, ast->file, ft_strlen(ast->file));
			write(STDERR_FILENO, "`)\n", 3);
			return (0);
		}
		if (!ft_strcmp(line, ast->file))
			return (free(line), 0);
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	return (0);
}
