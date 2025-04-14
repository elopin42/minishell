/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:01:50 by elopin            #+#    #+#             */
/*   Updated: 2025/04/11 05:52:13 by tbeauman         ###   ########.fr       */
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
	int		input_fd = STDIN_FILENO;
	int		pipe_fd[2];
	pid_t	pid;
	t_ast	*current = ast;
	
	// Pour chaque commande du pipeline sauf la dernière
	if (!current)
	{
		fd_printf(2, "%d, %s\n", __LINE__,__FILE__);
		return ;
	}
	while (current && current->type == NODE_PIPE) 
	{
		// Crée un pipe pour relier la commande courante à la suivante.
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		
		// Fork pour exécuter la commande courante (la partie gauche du pipe)
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			// Redirige l'entrée standard sur input_fd.
			if (input_fd != STDIN_FILENO)
			{
				if (dup2(input_fd, STDIN_FILENO) == -1)
				{
					perror("dup2 input_fd");
					exit(EXIT_FAILURE);
				}
				close(input_fd);
			}
			// Redirige la sortie vers l'extrémité écriture du pipe.
			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			{
				perror("dup2 pipe_fd[1]");
				exit(EXIT_FAILURE);
			}
			// Ferme les descripteurs inutilisés.
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			
			if (!current->left)
			{
				fd_printf(2, "%d %s\n", __LINE__,__FILE__);
				exit(EXIT_FAILURE);
			}
			// Exécute la commande courante (la partie gauche de current).
			close_all_fds();
			consume_tree(current->left, ms);

			cleanup(ms);
	
			exit(EXIT_SUCCESS);
		}
		// Dans le parent : 
		// Ferme input_fd s'il n'est pas STDIN_FILENO (issu d'une étape précédente).
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		// Ferme l'extrémité écriture du pipe (l'enfant l'utilise).
		close(pipe_fd[1]);
		// Le côté lecture devient l'entrée pour la prochaine commande.
		input_fd = pipe_fd[0];
		// Passe au nœud suivant dans l'AST.
		current = current->right;
	}

	if (!current)
	{
		fd_printf(2, "%d, %s\n", __LINE__,__FILE__);
		exit(EXIT_FAILURE);
	}
	// La dernière commande du pipeline (ou commande isolée)
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (input_fd != STDIN_FILENO)
		{
			if (dup2(input_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 last command input_fd");
				exit(EXIT_FAILURE);
			}
			close(input_fd);
		}
		close_all_fds();
		consume_tree(current, ms);

		cleanup(ms);

		exit(EXIT_SUCCESS);
	}
	else
	{
		// Dans le parent, ferme le dernier input_fd s'il a été dupliqué.
		
		// if (input_fd != STDIN_FILENO)
			close(input_fd);
		// Attend la fin de toutes les commandes (les enfants).
		while (wait(NULL) > 0)
			;
		// Restaure STDIN avec la sauvegarde d'origine.
		if (dup2(ms->saved_stdin, STDIN_FILENO) == -1)
		{
			perror("dup2 restore STDIN");
			exit(EXIT_FAILURE);
		}
		close(ms->saved_stdin);
		close_all_fds();
	}
}
