/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:18:45 by tbeauman          #+#    #+#             */
/*   Updated: 2025/03/18 17:20:15 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup(t_env *ms)
{
	free_tab(ms->array_tokens);
}

void	exit_clean(t_env *ms, int exit_code)
{
	cleanup(ms);
	if (ms->pididi != 0)
		kill(ms->pididi, SIGKILL);
	save_history();
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
	if ((*node)->file)
		free((*node)->file);
	free(*node);
	*node = NULL;
}
