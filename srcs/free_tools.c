/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:18:45 by tbeauman          #+#    #+#             */
/*   Updated: 2025/05/05 13:32:10 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup(t_env *ms)
{
	free(ms->cmd_line);
	ft_clear_tokens(&ms->tokens, &free);
	delete_ast(&ms->ast);
	free_tab(ms->envp);
	close_all_fds();
}

void	close_all_fds(void)
{
	int	i;

	i = 3;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}

void	exit_clean(t_env *ms, int exit_code)
{
	cleanup(ms);
	if (ms->pididi != 0)
		kill(ms->pididi, SIGKILL);
	save_history();
	ms->last_exit_code = exit_code;
	exit(exit_code);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
	{
		return ;
	}
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	delete_ast(t_ast **node)
{
	if (!*node)
		return ;
	delete_ast(&((*node)->left));
	delete_ast(&((*node)->right));
	if ((*node)->cmd)
		ft_clear_tokens(&((*node)->cmd), &free);
	if ((*node)->file_token)
		ft_clear_tokens(&(*node)->file_token, &free);
	free(*node);
	*node = NULL;
}
